#include "ui.h"
#include "display.h"

/* Starter screen: a title, a button with a tap counter and a backlight slider. */

static lv_obj_t *s_count_label;

static void btn_clicked_cb(lv_event_t *e)
{
    static uint32_t count;
    LV_UNUSED(e);
    lv_label_set_text_fmt(s_count_label, "Tapped %" LV_PRIu32 " times", ++count);
}

static void brightness_changed_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    bsp_display_brightness_set(lv_slider_get_value(slider));
}

void ui_init(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);

    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(scr, 20, 0);

    lv_obj_t *title = lv_label_create(scr);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_28, 0);
    lv_label_set_text(title, "JC3248W535");

    lv_obj_t *hint = lv_label_create(scr);
    lv_label_set_text(hint, "Edit src/ui/ui.c to get started");

    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_add_event_cb(btn, btn_clicked_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, LV_SYMBOL_OK " Tap me");

    s_count_label = lv_label_create(scr);
    lv_label_set_text(s_count_label, "Tapped 0 times");

    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, lv_pct(60));
    lv_slider_set_range(slider, 5, 100);
    lv_slider_set_value(slider, 100, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, brightness_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
