#include <inttypes.h>
#include <esp_log.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include "esp_bsp.h"
#include "display.h"
#include "lv_port.h"
#include "screen_off.h"
#include "ui/ui.h"

static const char *TAG = "main";

/* Display rotation: LV_DISP_ROT_NONE (portrait), _90, _180 or _270 */
#define APP_DISPLAY_ROTATION    LV_DISP_ROT_90

/* Backlight turns off after this much inactivity and wakes on touch; 0 keeps it on */
#define APP_SCREEN_OFF_MS       60000

void app_main(void)
{
    esp_chip_info_t chip_info;
    uint32_t flash_size = 0;
    esp_chip_info(&chip_info);
    esp_flash_get_size(NULL, &flash_size);
    ESP_LOGI(TAG, "%s rev v%d.%d, %d cores, %" PRIu32 " MB flash, %u KB PSRAM free",
             CONFIG_IDF_TARGET, chip_info.revision / 100, chip_info.revision % 100, chip_info.cores,
             flash_size / (1024 * 1024), heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
        .rotate = APP_DISPLAY_ROTATION,
    };
    if (bsp_display_start_with_config(&cfg) == NULL) {
        ESP_LOGE(TAG, "Display init failed");
        return;
    }
    bsp_display_backlight_on();

    if (APP_SCREEN_OFF_MS > 0 && screen_off_init(APP_SCREEN_OFF_MS) != ESP_OK) {
        ESP_LOGW(TAG, "screen_off_init failed");
    }

    /* LVGL is not thread-safe: hold the lock while touching LVGL objects outside LVGL callbacks */
    bsp_display_lock(0);
    ui_init();
    bsp_display_unlock();
}
