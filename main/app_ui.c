#include <inttypes.h>

#include "app_config.h"
#include "app_ui.h"

#include "bsp/esp-bsp.h"
#include "lvgl.h"

#define UI_COLOR_BG 0x000000
#define UI_COLOR_TEXT 0xEAF2F8
#define UI_COLOR_MUTED 0x7E8A94
#define UI_COLOR_DIM 0x303840
#define UI_COLOR_CYAN 0x00D5FF
#define UI_COLOR_GREEN 0x39D98A
#define UI_COLOR_AMBER 0xFFB84D
#define UI_COLOR_RED 0xFF5D5D

static lv_obj_t *status_label = NULL;
static lv_obj_t *detail_label = NULL;
static lv_obj_t *packets_label = NULL;
static lv_obj_t *rate_label = NULL;
static lv_obj_t *target_label = NULL;
static lv_obj_t *accent_bar = NULL;

static lv_obj_t *ui_create_label(lv_obj_t *parent, int32_t width, lv_text_align_t align, uint32_t color)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, width);
    lv_obj_set_style_text_align(label, align, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN);
    return label;
}

static void ui_set_accent(uint32_t color)
{
    if (accent_bar != NULL) {
        lv_obj_set_style_bg_color(accent_bar, lv_color_hex(color), LV_PART_MAIN);
    }
}

void app_ui_create_dashboard(void)
{
    if (!bsp_display_lock(0)) {
        return;
    }

    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(UI_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title = ui_create_label(screen, 140, LV_TEXT_ALIGN_LEFT, UI_COLOR_TEXT);
    lv_label_set_text(title, "CSI BEACON");
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 14, 12);

    lv_obj_t *mode = ui_create_label(screen, 110, LV_TEXT_ALIGN_RIGHT, UI_COLOR_MUTED);
    lv_label_set_text(mode, "LOW POWER");
    lv_obj_align(mode, LV_ALIGN_TOP_RIGHT, -14, 12);

    accent_bar = lv_obj_create(screen);
    lv_obj_remove_style_all(accent_bar);
    lv_obj_set_size(accent_bar, 292, 2);
    lv_obj_set_style_bg_opa(accent_bar, LV_OPA_COVER, LV_PART_MAIN);
    ui_set_accent(UI_COLOR_CYAN);
    lv_obj_align(accent_bar, LV_ALIGN_TOP_MID, 0, 39);

    status_label = ui_create_label(screen, 292, LV_TEXT_ALIGN_CENTER, UI_COLOR_CYAN);
    lv_label_set_text(status_label, "INICIANDO");
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, -46);

    detail_label = ui_create_label(screen, 292, LV_TEXT_ALIGN_CENTER, UI_COLOR_MUTED);
    lv_label_set_text(detail_label, "Pantalla lista\nPreparando Wi-Fi");
    lv_obj_align(detail_label, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *footer_line = lv_obj_create(screen);
    lv_obj_remove_style_all(footer_line);
    lv_obj_set_size(footer_line, 292, 1);
    lv_obj_set_style_bg_color(footer_line, lv_color_hex(UI_COLOR_DIM), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(footer_line, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_align(footer_line, LV_ALIGN_BOTTOM_MID, 0, -56);

    packets_label = ui_create_label(screen, 84, LV_TEXT_ALIGN_LEFT, UI_COLOR_TEXT);
    lv_label_set_text(packets_label, "TX\n--");
    lv_obj_align(packets_label, LV_ALIGN_BOTTOM_LEFT, 14, -16);

    rate_label = ui_create_label(screen, 76, LV_TEXT_ALIGN_CENTER, UI_COLOR_MUTED);
    lv_label_set_text_fmt(rate_label, "RATE\n%d/s", UDP_PACKETS_PER_SECOND);
    lv_obj_align(rate_label, LV_ALIGN_BOTTOM_MID, -3, -16);

    target_label = ui_create_label(screen, 134, LV_TEXT_ALIGN_RIGHT, UI_COLOR_MUTED);
    lv_label_set_text_fmt(target_label, "TARGET\n%s", DR_SIMI_RX_IP);
    lv_obj_align(target_label, LV_ALIGN_BOTTOM_RIGHT, -14, -16);

    bsp_display_unlock();
}

void app_ui_show_boot(void)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_CYAN);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_CYAN), LV_PART_MAIN);
    lv_label_set_text(status_label, "INICIANDO");
    lv_label_set_text(detail_label, "Pantalla lista\nPreparando Wi-Fi");
    lv_label_set_text(packets_label, "TX\n--");
    bsp_display_unlock();
}

void app_ui_show_connecting(int retry)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_AMBER);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_AMBER), LV_PART_MAIN);
    lv_label_set_text(status_label, "CONECTANDO WIFI");
    if (retry > 0) {
        lv_label_set_text_fmt(detail_label, "SSID %s\nIntento %d/%d", WIFI_SSID, retry, WIFI_MAXIMUM_RETRY);
    } else {
        lv_label_set_text_fmt(detail_label, "SSID %s\nBuscando punto de acceso", WIFI_SSID);
    }
    lv_label_set_text(packets_label, "TX\n--");
    bsp_display_unlock();
}

void app_ui_show_connected(const char *ip_addr)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_GREEN);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_GREEN), LV_PART_MAIN);
    lv_label_set_text(status_label, "ONLINE");
    lv_label_set_text_fmt(detail_label, "IP %s\nUDP listo para %s:%d", ip_addr, DR_SIMI_RX_IP, DR_SIMI_RX_PORT);
    lv_label_set_text(packets_label, "TX\nready");
    bsp_display_unlock();
}

void app_ui_show_wifi_failed(void)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_RED);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_RED), LV_PART_MAIN);
    lv_label_set_text(status_label, "SIN WIFI");
    lv_label_set_text(detail_label, "No se pudo unir a la red\nRevisa SSID, clave o senal");
    lv_label_set_text(packets_label, "TX\npausa");
    bsp_display_unlock();
}

void app_ui_show_transmitting(uint32_t packets_sent, const char *local_ip)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_GREEN);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_GREEN), LV_PART_MAIN);
    lv_label_set_text(status_label, "TRANSMITIENDO");
    lv_label_set_text_fmt(detail_label, "Destino %s:%d\nIP local %s", DR_SIMI_RX_IP, DR_SIMI_RX_PORT, local_ip);
    lv_label_set_text_fmt(packets_label, "TX\n%" PRIu32, packets_sent);
    bsp_display_unlock();
}

void app_ui_show_udp_error(int socket_errno)
{
    if (status_label == NULL || !bsp_display_lock(0)) {
        return;
    }

    ui_set_accent(UI_COLOR_RED);
    lv_obj_set_style_text_color(status_label, lv_color_hex(UI_COLOR_RED), LV_PART_MAIN);
    lv_label_set_text(status_label, "ERROR UDP");
    lv_label_set_text_fmt(detail_label, "Socket fallo con errno %d\nReintentando envio", socket_errno);
    bsp_display_unlock();
}
