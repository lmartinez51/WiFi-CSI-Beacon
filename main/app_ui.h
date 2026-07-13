#pragma once

#include <stdint.h>

void app_ui_create_dashboard(void);
void app_ui_show_boot(void);
void app_ui_show_connecting(int retry);
void app_ui_show_connected(const char *ip_addr);
void app_ui_show_wifi_failed(void);
void app_ui_show_transmitting(uint32_t packets_sent, const char *local_ip);
void app_ui_show_udp_error(int socket_errno);
