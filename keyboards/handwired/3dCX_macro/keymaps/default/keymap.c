
/* Copyright 2020 dsanchezseco
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


// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _00,
    _XL,
    _MO,
};


#define S_DOWN  S(KC_DOWN)
#define SC_DOWN S(C(KC_DOWN))
#define S_RGHT  S(KC_RGHT)
#define SC_RGHT S(C(KC_RGHT))


enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  SRCHSEL,
  KY_DEG,
  KY_DIA,
  KY_PLMN,
  KY_SUM,
  KY_MAX,
  KY_MIN,
  KY_AVG

};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_00] = LAYOUT_ortho_5x15(
    _______, _______, KC_CIRC,  KC_ESC, XXXXXXX, XXXXXXX, XXXXXXX,  KC_DEL,
      KC_P7,   KC_P8,   KC_P9, KC_SLSH, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL,
      KC_P4,   KC_P5,   KC_P6, KC_ASTR, XXXXXXX, XXXXXXX, XXXXXXX,    KC_S,
      KC_P1,   KC_P2,   KC_P3, KC_MINS, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN3,
      KC_P0,  NM_DOT, KC_PENT, KC_PLUS, KC_BTN2, MO(_MO), KC_BTN1, KC_BTN2),

    [_XL] = LAYOUT_ortho_5x15(
      KC_F4,   KC_F8, KC_DQUO, _______, XXXXXXX, XXXXXXX, XXXXXXX,   KC_DEL,
    _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX,  KC_LCTL,
    _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX,     KC_S,
    _______, _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX,  KC_BTN3,
    KC_LPRN, KC_RPRN,  KC_TAB,  KC_EQL, KC_BTN2, MO(_MO), KC_BTN1, KC_BTN2),

    [_MO] = LAYOUT_ortho_5x15(
    _______, _______,   KC_ESC, _______, XXXXXXX, XXXXXXX, XXXXXXX,  KC_DEL,
    KC_PGDN,   KC_UP,  KC_PGUP, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL,
    KC_LEFT, KC_DOWN, KC_RIGHT, _______, XXXXXXX, XXXXXXX, XXXXXXX,    KC_S,
    _______,  S_DOWN,   S_RGHT, _______, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN3,
    _______, SC_DOWN,  SC_RGHT, _______, KC_BTN2, MO(_MO), KC_BTN1, KC_BTN2),

  };



// Macro set up: ref //https://getreuer.info/posts/keyboards/macros/index.html
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case SELWORD:  // Selects the current word under the cursor.
        if (record->event.pressed) {
            SEND_STRING(SS_LCTL(SS_TAP(X_RGHT) SS_LSFT(SS_TAP(X_LEFT))));
            // Mac users, change LCTL to LALT:
            // SEND_STRING(SS_LALT(SS_TAP(X_RGHT) SS_LSFT(SS_TAP(X_LEFT))));
        }
            return false;
    case SRCHSEL:  // Searches the current selection in a new tab.
        if (record->event.pressed) {
            // Mac users, change LCTL to LGUI.
            SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
        }
            return false;
    case KY_DEG:  // Searches the current selection in a new tab.
        if (record->event.pressed) {
            // Mac users, change LCTL to LGUI.
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_7) SS_TAP(X_KP_6)));
        }
            return false;
    case KY_PLMN:  // Searches the current selection in a new tab.
        if (record->event.pressed) {
            // Mac users, change LCTL to LGUI.
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_7) SS_TAP(X_KP_7)));
        }
            return false;
    case KY_DIA:  // Searches the current selection in a new tab.
        if (record->event.pressed) {
            // Mac users, change LCTL to LGUI.
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_8)));
        }
            return false;
    case KY_SUM:  // Sum for Excel
        if (record->event.pressed) {
            SEND_STRING("=SUM()" SS_TAP(KC_LEFT));
        }
            return false;
    case KY_MAX:  // MAX for Excel
        if (record->event.pressed) {
            SEND_STRING("=MAX()" SS_TAP(X_LEFT));
            return false;
        }
    case KY_MIN:  // MIN for Excel
        if (record->event.pressed) {
            SEND_STRING("=MIN()" SS_TAP(X_LEFT));
            return false;
        }
    case KY_AVG:  // AVERAGE for Excel
        if (record->event.pressed) {
            SEND_STRING("=AVERAGE()" SS_TAP(X_LEFT));
            return false;
    }
  return true;
}

#ifdef ENCODER_MAP_ENABLE
    const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {0};
#endif


#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_00] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN)        },
    [_XL] =  { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT)     },
    [_MO] =  { ENCODER_CCW_CW(KC_WH_D, KC_WH_U)      },
 ;
#endif

