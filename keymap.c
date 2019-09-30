#include QMK_KEYBOARD_H
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};

#define KC_ KC_NO
#define KC_____ KC_TRNS
#define KC_XXXX KC_NO

#define KC_C_Z LCTL(KC_Z)
#define KC_LOW LOWER
#define KC_RAI RAISE

#define KC_LSPC LT(_LOWER, KC_SPC)
#define KC_RSPC LT(_RAISE, KC_SPC)
#define KC_RENT LT(_RAISE, KC_ENT)
#define KC_RBSP LT(_RAISE, KC_BSPC)
#define KC_SPFT SFT_T(KC_SPC)
#define KC_S_EN SFT_T(KC_ENT)
#define KC_ALBS ALT_T(KC_BSPC)
#define KC_ALEN ALT_T(KC_ENT)
#define KC_DCTL CTL_T(KC_DEL)

#define KC_VU KC__VOLUP
#define KC_VD KC__VOLDOWN
#define KC_MU KC__MUTE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_kc( \
  //,----+----+----+----+----+----.                        ,----+----+----+----+----+----.
     TAB , Q  , W  , E  , R  , T  ,                          Y  , U  , I  , O  , P  ,MINS,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LCTL, A  , S  , D  , F  , G  ,                          H  , J  , K  , L  ,SCLN,QUOT,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LSFT, Z  , X  , C  , V  , B  ,                          N  , M  ,COMM,DOT ,SLSH,BSLS,
  //`----+----+----+----+----+----+----+----.    ,----+----+----+----+----+----+----+----'
                         LALT,LCMD,LSPC,LSFT,     DCTL,RSPC,S_EN,BSPC
  //                    `----+----+----+----'    `----+----+----+----'
  ),

  [_LOWER] = LAYOUT_kc(
  //,----+----+----+----+----+----.                        ,----+----+----+----+----+----.
     ESC , 1  , 2  , 3  , 4  , 5  ,                          6  , 7  , 8  , 9  , 0  ,    ,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LCTL,EXLM,AT  ,HASH,DLR ,PERC,                         CIRC,AMPR,ASTR,LPRN,RPRN,    ,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LSFT,PLUS,EQL ,LCBR,LBRC,LPRN,                         RPRN,RBRC,RCBR,GRV ,TILD,    ,
  //`----+----+----+----+----+----+----+----.    ,----+----+----+----+----+----+----+----'
                         LALT,LCMD,    ,LSFT,     DCTL,    ,S_EN,BSPC
  //                    `----+----+----+----'    `----+----+----+----'
  ),

  [_RAISE] = LAYOUT_kc(
  //,----+----+----+----+----+----.                        ,----+----+----+----+----+----.
     ESC , F1 , F2 , F3 , F4 , F5 ,                          F6 , F7 , F8 , F9 ,F10 ,F11 ,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LCTL, VD , VU , MU ,    ,    ,                         LEFT,DOWN, UP ,RGHT,    ,F12 ,
  //|----+----+----+----+----+----|                        |----+----+----+----+----+----|
     LSFT,BRMD,BRMU,    ,    ,    ,                         HOME,PGDN,PGUP,END ,    ,    ,
  //`----+----+----+----+----+----+----+----.    ,----+----+----+----+----+----+----+----'
                         LALT,LCMD,    ,LSFT,     DCTL,    ,S_EN,BSPC
  //                    `----+----+----+----'    `----+----+----+----'
  )
};

void matrix_init_user(void) {
  //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
  #ifdef SSD1306OLED
    iota_gfx_init(!has_usb());   // turns on the display
  #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_scan_user(void) {
  iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
      }
      return false;
      break;
  }
  return true;
}
