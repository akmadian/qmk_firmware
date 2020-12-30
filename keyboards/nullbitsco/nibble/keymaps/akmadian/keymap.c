/* Copyright 2020 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "micro_oled.h"

#define _BASE     0
#define _VIA1     1


#define MATRIX_DISPLAY_X 0
#define MATRIX_DISPLAY_Y 16
#define LOCK_DISPLAY_X 45
#define LOCK_DISPLAY_Y 18
#define MOD_DISPLAY_X 74
#define MOD_DISPLAY_Y 18
#define LAYER_DISPLAY_X 39
#define LAYER_DISPLAY_Y 0
#define ENC_DISPLAY_X 86
#define ENC_DISPLAY_Y 0

bool led_numlock = false;
bool led_capslock = false;
bool led_scrolllock = false;

bool oled_off = false;
uint8_t enc_mode = 0;
char *enc_str[] = {"VOL", "SCR", "BRT"};
uint16_t enc_keys[2][3] = {
    { KC_AUDIO_VOL_UP, KC_UP, KC_BRIGHTNESS_UP },
    { KC_AUDIO_VOL_DOWN, KC_DOWN, KC_BRIGHTNESS_DOWN }
};

bool send_oled = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_ansi(
             KC_ESC , KC_1,    KC_2,    KC_3,   KC_4,  KC_5,  KC_6,    KC_7,    KC_8,    KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_GRV, 
    KC_MUTE, KC_TAB , KC_Q,    KC_W,    KC_E,   KC_R,  KC_T,  KC_Y,    KC_U,    KC_I,    KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, 
    MO(1)  , KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,  KC_G,  KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_HOME, 
    MO(2)  , KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,  KC_B,  KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END, 
    MO(3)  , KC_LCTL, KC_LGUI, KC_LALT,                KC_SPC,                  KC_RALT, KC_NO, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [_VIA1] = LAYOUT_ansi(
             KC_TRNS, KC_F1,   KC_F2,   KC_F3,  KC_F4, KC_F5, KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10, KC_F11, KC_F12, KC_HOME, KC_INS, 
    RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_POWER, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_SLEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_D, KC_WH_L, KC_WH_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_L, KC_WH_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT
  ),
};

void matrix_init_kb(void) {
    matrix_init_user();
}

void draw_keyboard_outline(void) {
    draw_rect_soft(MATRIX_DISPLAY_X, MATRIX_DISPLAY_Y, 35, 13, PIXEL_ON, NORM);
    draw_line_hori(MATRIX_DISPLAY_X + 2, 15, 4, PIXEL_OFF, NORM);
}

void draw_keyboard_matrix(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        for (uint8_t y = 0; y < MATRIX_COLS; y++) {
            draw_pixel(MATRIX_DISPLAY_X + y*2 + 2, MATRIX_DISPLAY_Y + x*2 + 2,(matrix_get_row(x) & (1 << y)) > 0, NORM);
        }
    }
}

void draw_keyboard_locks(void) {
    led_t led_state = host_keyboard_led_state();
    if (led_state.caps_lock) {
        draw_rect_filled_soft(LOCK_DISPLAY_X + 0, LOCK_DISPLAY_Y, 5 + (3 * 6), 11, PIXEL_ON, NORM);
        draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y +2, "CAP", PIXEL_OFF, NORM, 0);
    } else if (led_capslock == false) {
        draw_rect_filled_soft(LOCK_DISPLAY_X + 0, LOCK_DISPLAY_Y, 5 + (3 * 6), 11, PIXEL_OFF, NORM);
        draw_rect_soft(LOCK_DISPLAY_X + 0, LOCK_DISPLAY_Y, 5 + (3 * 6), 11, PIXEL_ON, NORM);
        draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y +2, "CAP", PIXEL_ON, NORM, 0);
    }
}

void draw_keyboard_mods(void) {
    uint8_t mods = get_mods();

    if (mods & MOD_MASK_SHIFT) {
        draw_rect_filled_soft(MOD_DISPLAY_X + 0, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 3, MOD_DISPLAY_Y + 2, "S", PIXEL_OFF, NORM, 0);
    } else {
        draw_rect_filled_soft(MOD_DISPLAY_X + 0, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_OFF, NORM);
        draw_rect_soft(MOD_DISPLAY_X + 0, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 3, MOD_DISPLAY_Y + 2, "S", PIXEL_ON, NORM, 0);
    }
    if (mods & MOD_MASK_CTRL) {
        draw_rect_filled_soft(MOD_DISPLAY_X + 14, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 17, MOD_DISPLAY_Y + 2, "C", PIXEL_OFF, NORM, 0);
    } else {
        draw_rect_filled_soft(MOD_DISPLAY_X + 14, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_OFF, NORM);
        draw_rect_soft(MOD_DISPLAY_X + 14, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 17, MOD_DISPLAY_Y + 2, "C", PIXEL_ON, NORM, 0);
    }
    if (mods & MOD_MASK_ALT) {
        draw_rect_filled_soft(MOD_DISPLAY_X + 28, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 31, MOD_DISPLAY_Y + 2, "A", PIXEL_OFF, NORM, 0);
    } else {
        draw_rect_filled_soft(MOD_DISPLAY_X + 28, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_OFF, NORM);
        draw_rect_soft(MOD_DISPLAY_X + 28, MOD_DISPLAY_Y, 5 + (1 * 6), 11, PIXEL_ON, NORM);
        draw_string(MOD_DISPLAY_X + 31, MOD_DISPLAY_Y + 2, "A", PIXEL_ON, NORM, 0);
    }
}


void draw_layer_section(int8_t startX, int8_t startY, bool show_legend){
  if(show_legend){
    draw_string(startX + 1, startY + 2, "LAYER", PIXEL_ON, NORM, 0);
  } else {
    startX -= 32;
  }
  draw_rect_filled_soft(startX + 32, startY + 1, 9, 9, PIXEL_ON, NORM);
  draw_char(startX + 34, startY + 2, get_highest_layer(layer_state) + 0x30, PIXEL_ON, XOR, 0);
}

void draw_encoder(int8_t startX, int8_t startY, bool show_legend){
  if(show_legend){
    draw_string(startX + 1, startY + 2, "ENC", PIXEL_ON, NORM, 0);
  } else {
    startX -= 22;
  }
  draw_rect_filled_soft(startX + 22, startY + 1, 3 + (3 * 6), 9, PIXEL_ON, NORM);
  char* mode_string = "";
  switch(get_highest_layer(layer_state)){
    default:
    case 0:
      mode_string = "VOL";
      break;
    case 1:
      mode_string = "RGB";
      break;
    case 2:
      mode_string = "UDF";
      break;
    case 3:
      mode_string = "UDF";
      break;
  }
  draw_string(startX + 24, startY + 2, mode_string, PIXEL_ON, XOR, 0);
}

void init_oled(void) {
    clear_buffer();
    draw_keyboard_outline();
    draw_keyboard_locks();
    draw_keyboard_mods();
    draw_layer_section(0, 0, true);
    draw_encoder(45, 0, true);
}

void draw_display(void) {
    draw_keyboard_matrix();
    draw_keyboard_mods();
    draw_keyboard_locks();
    draw_layer_section(0, 0, true);
    draw_encoder(45, 0, true);
    send_buffer();
}

void matrix_scan_kb(void) {
    if (send_oled) {
        draw_display();
        send_oled = false;
    }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    send_oled = true;
    return process_record_user(keycode, record);
}

void keyboard_post_init_user(void) {
    init_oled();
}

void encoder_update_kb(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case 0:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
            break;
        case 1:
            if (true) {
            uint8_t mods = get_mods();

            if (clockwise) {
                if (mods & MOD_MASK_ALT) {
                    rgblight_increase_val();
                } else if (mods & MOD_MASK_CTRL) {
                    rgblight_increase_speed();
                } else {
                    rgblight_step();
                }
            } else {
                if (mods & MOD_MASK_ALT) {
                    rgblight_decrease_val();
                } else if (mods & MOD_MASK_CTRL) {
                    rgblight_increase_speed();
                } else {
                    rgblight_step_reverse();
                }
            }
            }
            break;
    }
}

