#pragma once

#include "esp_err.h"
#include "lvgl.h"

esp_err_t app_display_start_without_touch(lv_display_t **ret_display);
esp_err_t app_display_set_brightness(int brightness_percent);
