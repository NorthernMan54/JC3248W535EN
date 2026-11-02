#include "screen_off.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_bsp.h"
#include "display.h"

static const char *TAG = "SCREEN_OFF";
static esp_timer_handle_t s_screen_off_timer = NULL;
static int64_t s_last_activity_ts_us = 0;
static uint32_t s_timeout_ms = 60000;
static bool s_screen_off = false;

static void screen_off_timer_cb(void *arg)
{
    (void)arg;

    int64_t now = esp_timer_get_time();
    if (!s_screen_off && s_last_activity_ts_us > 0) {
        int64_t elapsed_ms = (now - s_last_activity_ts_us) / 1000;
        if (elapsed_ms >= (int64_t)s_timeout_ms) {
            ESP_LOGI(TAG, "Inactivity %lldms reached, turning display off", elapsed_ms);
            bsp_display_backlight_off();
            s_screen_off = true;
        }
    }
}

esp_err_t screen_off_init(uint32_t timeout_ms)
{
    if (timeout_ms == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    s_timeout_ms = timeout_ms;
    s_screen_off = false;
    s_last_activity_ts_us = esp_timer_get_time();

    if (s_screen_off_timer) {
        esp_timer_stop(s_screen_off_timer);
        esp_timer_delete(s_screen_off_timer);
        s_screen_off_timer = NULL;
    }

    const esp_timer_create_args_t timer_args = {
        .callback = screen_off_timer_cb,
        .name = "screen_off_timer"
    };

    esp_err_t err = esp_timer_create(&timer_args, &s_screen_off_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create timer: %s", esp_err_to_name(err));
        return err;
    }

    err = esp_timer_start_periodic(s_screen_off_timer, 1000 * 1000);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start timer: %s", esp_err_to_name(err));
        esp_timer_delete(s_screen_off_timer);
        s_screen_off_timer = NULL;
        return err;
    }

    return ESP_OK;
}

void screen_off_deinit(void)
{
    if (s_screen_off_timer) {
        esp_timer_stop(s_screen_off_timer);
        esp_timer_delete(s_screen_off_timer);
        s_screen_off_timer = NULL;
    }
    s_screen_off = false;
}

void screen_off_activity(void)
{
    s_last_activity_ts_us = esp_timer_get_time();
    if (s_screen_off) {
        ESP_LOGI(TAG, "Wake-up from touch activity");
        bsp_display_backlight_on();
        s_screen_off = false;
    }
}

bool screen_off_is_off(void)
{
    return s_screen_off;
}
