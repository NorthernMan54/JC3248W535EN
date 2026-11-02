#include "ui.h"
#include "lang_images.h"

/*
 * Starter screen: "Let's Build" in the ten most spoken languages.
 * The non-Latin phrases are pre-rendered alpha masks (see tools/gen_lang_images.py),
 * tinted here at runtime. Tap anywhere to replay the intro.
 */

#define CELL_HEIGHT     46
#define FADE_TIME_MS    400
#define FADE_STAGGER_MS 110

static const uint32_t s_colors[LANG_IMAGE_COUNT] = {
    0xE57373, 0xFF8A65, 0xFFB74D, 0xFFD54F, 0xAED581, 0x4DB6AC, 0x4FC3F7, 0x7986CB, 0xBA68C8,
};

/* Title followed by one cell per language, in fade-in order */
static lv_obj_t *s_items[1 + LANG_IMAGE_COUNT];

static void set_opa(void *obj, int32_t v)
{
    lv_obj_set_style_opa(obj, (lv_opa_t)v, 0);
}

static void play_intro(void)
{
    for (int i = 0; i < 1 + LANG_IMAGE_COUNT; i++) {
        lv_obj_set_style_opa(s_items[i], LV_OPA_TRANSP, 0);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, s_items[i]);
        lv_anim_set_exec_cb(&a, set_opa);
        lv_anim_set_values(&a, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_anim_set_time(&a, FADE_TIME_MS);
        lv_anim_set_delay(&a, i * FADE_STAGGER_MS);
        lv_anim_start(&a);
    }
}

static void screen_clicked_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    play_intro();
}

static lv_obj_t *create_cell(lv_obj_t *parent, const lang_image_t *lang, lv_color_t color)
{
    lv_obj_t *cell = lv_obj_create(parent);
    lv_obj_remove_style_all(cell);
    lv_obj_clear_flag(cell, LV_OBJ_FLAG_CLICKABLE);  /* let taps reach the screen */
    lv_obj_set_size(cell, lv_pct(50), CELL_HEIGHT);
    lv_obj_set_flex_flow(cell, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cell, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cell, 2, 0);

    lv_obj_t *phrase = lv_img_create(cell);
    lv_img_set_src(phrase, lang->img);
    lv_obj_set_style_img_recolor(phrase, color, 0);
    lv_obj_set_style_img_recolor_opa(phrase, LV_OPA_COVER, 0);

    lv_obj_t *name = lv_label_create(cell);
    lv_label_set_text(name, lang->language);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(name, lv_color_hex(0x8A8A8A), 0);

    return cell;
}

void ui_init(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);

    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_all(scr, 0, 0);
    lv_obj_set_style_pad_gap(scr, 0, 0);  /* the theme's gap would wrap the 50% cells to one per row */
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_event_cb(scr, screen_clicked_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "Let's Build");
    lv_obj_set_width(title, lv_pct(100));
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_pad_bottom(title, 10, 0);
    s_items[0] = title;

    for (int i = 0; i < LANG_IMAGE_COUNT; i++) {
        s_items[1 + i] = create_cell(scr, &lang_images[i], lv_color_hex(s_colors[i]));
    }

    play_intro();
}
