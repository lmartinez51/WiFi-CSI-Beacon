#pragma once

#include "esp_err.h"

typedef struct {
    const char *ssid;
    const char *password;
    int max_retries;
    void (*on_connecting)(int retry);
    void (*on_connected)(const char *ip_addr);
    void (*on_failed)(void);
} app_wifi_config_t;

esp_err_t app_wifi_init_sta(const app_wifi_config_t *config);
const char *app_wifi_get_local_ip(void);
