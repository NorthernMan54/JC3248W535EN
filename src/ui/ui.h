#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Build the UI. Called once from app_main() with the LVGL lock held.
 *
 * Replace this folder with a SquareLine Studio export (which also provides ui_init())
 * or with your own screens.
 */
void ui_init(void);

#ifdef __cplusplus
}
#endif
