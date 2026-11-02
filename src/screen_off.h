#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize screen off manager.
 * @param timeout_ms inactivity timeout in milliseconds (e.g. 60000 for 60s)
 * @return ESP_OK or error
 */
esp_err_t screen_off_init(uint32_t timeout_ms);

/**
 * Deinitialize screen off manager.
 */
void screen_off_deinit(void);

/**
 * Notify screen off manager about user activity (touch).
 */
void screen_off_activity(void);

/**
 * Check if screen is currently off.
 */
bool screen_off_is_off(void);

#ifdef __cplusplus
}
#endif
