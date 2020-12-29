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

#undef OLED_TIMEOUT
#define OLED_TIMEOUT 10000

#define TAPPING_TERM 150

#define MATRIX_DISPLAY_X 5
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
uint8_t layer;


enum custom_keycodes {
  OLED_TOGGLE = SAFE_RANGE,
  LI, // LinkedIn
  GH, // GitHub
  WS, // Website
  PN, // Phone Number
  EM  // Email Address
};

bool m_oled_on = true;

// Macro variables
/*
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
bool muted = false;
bool deafened = false;
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_BASE] = LAYOUT_ansi(
             KC_ESC , KC_1,    KC_2,    KC_3,   KC_4,  KC_5,  KC_6,    KC_7,    KC_8,    KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_GRV, 
    KC_MUTE, KC_TAB , KC_Q,    KC_W,    KC_E,   KC_R,  KC_T,  KC_Y,    KC_U,    KC_I,    KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, 
    MO(1)  , KC_CAPS, KC_A,    KC_S,    KC_D,   KC_F,  KC_G,  KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_HOME, 
    KC_NO  , KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,  KC_B,  KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END, 
    KC_NO  , KC_LCTL, KC_LGUI, KC_LALT,                KC_SPC,                  KC_RALT, KC_NO, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),
	[_VIA1] = LAYOUT_ansi(
             KC_TRNS, KC_F1,   KC_F2,   KC_F3,  KC_F4, KC_F5, KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10, KC_F11, KC_F12, KC_HOME, KC_INS, 
    RGB_TOG, KC_TRNS, KC_TRNS, WS, EM, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, OLED_TOGGLE, KC_POWER, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_SLEP, KC_TRNS, KC_TRNS, GH, KC_WH_D, KC_WH_L, KC_WH_R, LI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, PN, KC_TRNS, KC_WH_L, KC_WH_R, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT
  ),
};

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_90; }

void oled_task_user(void) {
    if (m_oled_on) {
      oled_write_P(PSTR("Layer"), false);
      switch (get_highest_layer(layer_state)) {
          case _BASE:
              oled_write_ln_P(PSTR("1\n"), false);
              break;
          case _VIA1:
              oled_write_ln_P(PSTR("2\n"), false);
              break;
          default:
              oled_write_ln_P(PSTR("UNDEF"), false);
      }

      int currwpm = get_current_wpm();
      char wpm_text[5];
      sprintf(wpm_text,"wpm\n%i",currwpm);
      oled_write_ln(wpm_text, false);

      if (layer_state_is(1)) {
        char rgb_mode[5];
        sprintf(rgb_mode, "\nrgb:%i", rgblight_get_mode());
        oled_write_ln(rgb_mode, false);
      } else {
        oled_write_ln_P(PSTR("     "), false);
      }

      led_t led_state = host_keyboard_led_state();
      oled_write_P(led_state.caps_lock ? PSTR("CAPS") : PSTR("    "), false);

      draw_string(LAYER_DISPLAY_X, LAYER_DISPLAY_Y + 2, "LAYER", PIXEL_ON, NORM, 0);
      draw_rect_filled_soft(LAYER_DISPLAY_X + 31, LAYER_DISPLAY_Y, 11, 11, PIXEL_ON, NORM);
      draw_char(LAYER_DISPLAY_X + 34, LAYER_DISPLAY_Y + 2, layer + 0x30, PIXEL_ON, XOR, 0);
    } else {
      oled_off();
    }
}

void init_oled(void) {
    clear_buffer();
    draw_keyboard_layer();
}

void draw_display(void) {
    draw_keyboard_layer();
    send_buffer();
}



void encoder_update_kb(uint8_t index, bool clockwise) {
  if (layer_state_is(0)) {
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
  }
  else if (layer_state_is(1)) {
    bool alt = get_mods() & MOD_MASK_ALT;
    bool ctrl = get_mods() & MOD_MASK_CTRL;

    if (clockwise) {
      if (alt) {
        rgblight_increase_speed();
      } else if (ctrl) {
        rgblight_increase_val();
      } else {
        rgblight_step();
      }
    } else {
      if (alt) {
        rgblight_decrease_speed();
      } else if (ctrl) {
        rgblight_decrease_val();
      } else {
        rgblight_step_reverse();
      }
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // Send keystrokes to host keyboard, if connected (see readme)
  process_record_remote_kb(keycode, record);
  switch(keycode) {
    case OLED_TOGGLE:
    if (record->event.pressed) {
      m_oled_on = !m_oled_on;
    }
    break;
    case LI:
    if (record->event.pressed) {
      SEND_STRING("https://www.linkedin.com/in/arimadian/");
    }
    break;
    case GH:
    if (record->event.pressed) {
      SEND_STRING("https://github.com/akmadian");
    }
    break;
    case WS:
    if (record->event.pressed) {
      SEND_STRING("https://arimadian.info/");
    }
    break;
    case PN:
    if (record->event.pressed) {
      SEND_STRING("2532496636");
    }
    break;
    case EM:
    if (record->event.pressed) {
      SEND_STRING("akmadian@gmail.com");
    }
    break;
  }
  return true;
}

void raw_hid_receive(uint8_t *data, uint8_t length) 
{
    oled_write_ln((char *)data, false);
}
