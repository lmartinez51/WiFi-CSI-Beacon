#include <stdint.h>

#include "app_display.h"

#include "bsp/esp-bsp.h"
#include "esp_check.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"

static const char *TAG = "APP_DISPLAY";

esp_err_t app_display_start_without_touch(lv_display_t **ret_display)
{
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;

    ESP_RETURN_ON_FALSE(ret_display != NULL, ESP_ERR_INVALID_ARG, TAG, "Display output handle is NULL");

    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "Failed to initialize LVGL port");

    const bsp_display_config_t display_config = {
        .max_transfer_sz = BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(bsp_display_new(&display_config, &panel_handle, &io_handle),
                        TAG, "Failed to initialize LCD panel");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(panel_handle, true),
                        TAG, "Failed to turn LCD panel on");

    const lvgl_port_display_cfg_t lvgl_display_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT,
#if CONFIG_BSP_LCD_DRAW_BUF_DOUBLE
        .double_buffer = true,
#else
        .double_buffer = false,
#endif
        .hres = BSP_LCD_H_RES,
        .vres = BSP_LCD_V_RES,
        .monochrome = false,
        .rotation = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = true,
        },
#if LVGL_VERSION_MAJOR >= 9
        .color_format = LV_COLOR_FORMAT_RGB565,
#endif
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
#if LVGL_VERSION_MAJOR >= 9
            .swap_bytes = (BSP_LCD_BIGENDIAN != 0),
#endif
        },
    };

    *ret_display = lvgl_port_add_disp(&lvgl_display_cfg);
    ESP_RETURN_ON_FALSE(*ret_display != NULL, ESP_FAIL, TAG, "Failed to add LVGL display");

    return ESP_OK;
}

esp_err_t app_display_set_brightness(int brightness_percent)
{
    return bsp_display_brightness_set(brightness_percent);
}
