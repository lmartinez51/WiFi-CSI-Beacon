#pragma once

#include <stdint.h>

#include "esp_err.h"

typedef struct {
    const char *target_ip;
    uint16_t target_port;
    const char *payload;
    uint32_t interval_ms;
    const char *local_ip;
    void (*on_transmitting)(uint32_t packets_sent, const char *local_ip);
    void (*on_error)(int socket_errno);
} udp_blaster_config_t;

esp_err_t udp_blaster_start(const udp_blaster_config_t *config);
