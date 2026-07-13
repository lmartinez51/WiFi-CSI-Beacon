#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include "udp_blaster.h"

#include "esp_check.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

static const char *TAG = "UDP_BLASTER";
static TaskHandle_t s_udp_task;
static udp_blaster_config_t s_config;

static void udp_blaster_task(void *param)
{
    struct sockaddr_in dest_addr = {
        .sin_addr.s_addr = inet_addr(s_config.target_ip),
        .sin_family = AF_INET,
        .sin_port = htons(s_config.target_port),
    };
    const uint32_t update_period_packets = (s_config.interval_ms > 0)
                                               ? (1000 / s_config.interval_ms)
                                               : 1;
    uint32_t packets_sent = 0;

    while (1) {
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create UDP socket: errno %d", errno);
            if (s_config.on_error != NULL) {
                s_config.on_error(errno);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        ESP_LOGI(TAG, "UDP blaster sending to %s:%d", s_config.target_ip, s_config.target_port);
        if (s_config.on_transmitting != NULL) {
            s_config.on_transmitting(packets_sent, s_config.local_ip);
        }

        while (1) {
            int err = sendto(sock, s_config.payload, strlen(s_config.payload), 0,
                             (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "UDP send failed: errno %d", errno);
                if (s_config.on_error != NULL) {
                    s_config.on_error(errno);
                }
                break;
            }

            packets_sent++;
            if ((packets_sent % 250) == 0) {
                ESP_LOGI(TAG, "Sent %" PRIu32 " calibration packets", packets_sent);
            }

            if ((packets_sent % update_period_packets) == 0 && s_config.on_transmitting != NULL) {
                s_config.on_transmitting(packets_sent, s_config.local_ip);
            }

            vTaskDelay(pdMS_TO_TICKS(s_config.interval_ms));
        }

        shutdown(sock, 0);
        close(sock);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t udp_blaster_start(const udp_blaster_config_t *config)
{
    ESP_RETURN_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, TAG, "UDP config is NULL");
    ESP_RETURN_ON_FALSE(config->target_ip != NULL, ESP_ERR_INVALID_ARG, TAG, "UDP target IP is NULL");
    ESP_RETURN_ON_FALSE(config->payload != NULL, ESP_ERR_INVALID_ARG, TAG, "UDP payload is NULL");
    ESP_RETURN_ON_FALSE(config->interval_ms > 0, ESP_ERR_INVALID_ARG, TAG, "UDP interval must be positive");
    ESP_RETURN_ON_FALSE(s_udp_task == NULL, ESP_ERR_INVALID_STATE, TAG, "UDP blaster task is already running");

    s_config = *config;

    BaseType_t ok = xTaskCreatePinnedToCore(udp_blaster_task,
                                            "udp_blaster",
                                            4096,
                                            NULL,
                                            tskIDLE_PRIORITY + 2,
                                            &s_udp_task,
                                            1);
    ESP_RETURN_ON_FALSE(ok == pdPASS, ESP_ERR_NO_MEM, TAG, "Failed to create UDP blaster task");

    return ESP_OK;
}
