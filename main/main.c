#include <stdint.h>

#include "app_config.h"
#include "app_display.h"
#include "app_ui.h"
#include "app_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "udp_blaster.h"

static const char *TAG = "CSI_BEACON";

static void on_wifi_connecting(int retry)
{
    app_ui_show_connecting(retry);
}

static void on_wifi_connected(const char *ip_addr)
{
    app_ui_show_connected(ip_addr);
}

static void on_wifi_failed(void)
{
    app_ui_show_wifi_failed();
}

static void on_udp_transmitting(uint32_t packets_sent, const char *local_ip)
{
    app_ui_show_transmitting(packets_sent, local_ip);
}

static void on_udp_error(int socket_errno)
{
    app_ui_show_udp_error(socket_errno);
}

void app_main(void)
{
    lv_display_t *display = NULL;
    ESP_ERROR_CHECK(app_display_start_without_touch(&display));
    (void)display;

    ESP_ERROR_CHECK(app_display_set_brightness(DISPLAY_BRIGHTNESS_PERCENT));

    app_ui_create_dashboard();
    app_ui_show_boot();
    app_ui_show_connecting(0);

    const app_wifi_config_t wifi_config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
        .max_retries = WIFI_MAXIMUM_RETRY,
        .on_connecting = on_wifi_connecting,
        .on_connected = on_wifi_connected,
        .on_failed = on_wifi_failed,
    };
    esp_err_t wifi_ret = app_wifi_init_sta(&wifi_config);
    if (wifi_ret != ESP_OK) {
        ESP_LOGE(TAG, "Wi-Fi setup failed: %s", esp_err_to_name(wifi_ret));
        app_ui_show_wifi_failed();
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    app_ui_show_transmitting(0, app_wifi_get_local_ip());

    const udp_blaster_config_t udp_config = {
        .target_ip = DR_SIMI_RX_IP,
        .target_port = DR_SIMI_RX_PORT,
        .payload = UDP_PAYLOAD,
        .interval_ms = UDP_BLAST_INTERVAL_MS,
        .local_ip = app_wifi_get_local_ip(),
        .on_transmitting = on_udp_transmitting,
        .on_error = on_udp_error,
    };
    esp_err_t udp_ret = udp_blaster_start(&udp_config);
    if (udp_ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start UDP blaster: %s", esp_err_to_name(udp_ret));
        app_ui_show_udp_error((int)udp_ret);
    }
}
