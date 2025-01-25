/* Copyright 2024 aspot
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

 qmk lint -kb handwired/3dcx_macro
 qmk compile -kb handwired/3dcx_macro -km default
 qmk flash -kb handwired/3dcx_macro -km default
 */

#include <stdatomic.h>
#include QMK_KEYBOARD_H


// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _00,
    _MO,
    _FN,
    _XL
};

#define ENT_FN  LT(_FN, KC_PENT)
#define SPC_MO  LT(_MO,KC_SPC)
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
 /*
    ┌─────┬─────┬─────┬─────┬───────────────────────┬─────┐
    | tab |home │ del | esc |                       | tab |
    ├─────┼─────┼─────┼─────┤                       ├─────┤
    |  6  |  8  |  9  |  /  |                       | rmb |
    ├─────┼─────┼─────┼─────┤                       ├─────┤
    |  4  |  5  |  6  |  *  |                       | sft |
    ├─────┼─────┼─────┼─────┤                       ├─────┤
    |  1  |  2  |  3  |  +  |                       | ctl |
    ├─────┼─────┼─────┼─────┤─────┬─────┬─────┬─────┼─────┤
    |  0  |  .  |ent.F| MO  ( rmb ) XL  | LMB |  S  | MMB |
    └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
*/
    [_00] = LAYOUT_ortho_5x6(
    KC_TAB, KC_HOME, KC_DEL, KC_ESC,  XXXXXXX, KC_TAB,
    KC_P7,  KC_P8,   KC_P9,  KC_SLSH, KC_BTN2, KC_BTN2,
    KC_P4,  KC_P5,   KC_P6,  KC_ASTR, MO(_XL), KC_LSFT,
    KC_P1,  KC_P2,   KC_P3,  KC_PLUS, KC_BTN1, KC_LCTL,
    KC_P0,  KC_PDOT, ENT_FN, SPC_MO,  KC_S,    KC_BTN3
    ),

    [_MO] = LAYOUT_ortho_5x6(
    _______, KC_END,  KC_DEL,  _______, XXXXXXX, _______,
    KC_PGDN, KC_UP,   KC_PGUP, _______, _______, _______,
    KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_R,    _______,
    _______, SC_DOWN, SC_RGHT, _______, _______, _______,
    KC_SPC,  KC_PDOT, KC_EQL,  KC_TRNS, _______, _______
    ),

    [_FN] = LAYOUT_ortho_5x6(
    KC_F10,  KC_F11,  KC_F12,  _______, XXXXXXX, _______,
    KC_F7,   KC_F8,   KC_F9,   _______, _______, _______,
    KC_F4,   KC_F5,   KC_F6,   _______, _______, _______,
    KC_F1,   KC_F2,   KC_F3,   _______, _______, _______,
    _______, _______, _______, _______, _______, _______
    ),

    [_XL] = LAYOUT_ortho_5x6(
    _______, _______, _______, _______, XXXXXXX, KC_TAB,
    _______, _______, _______, _______, _______, KC_BTN2,
    _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, KC_TRNS, _______,
    KC_LPRN, KC_RPRN, KC_EQL, SPC_MO,   KC_BTN2, KC_BTN3
    )
  };



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_00] =  { ENCODER_CCW_CW(KC_UP,   KC_DOWN) },
    [_MO] =  { ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_XL] =  { ENCODER_CCW_CW(KC_LEFT, KC_RGHT) },
};
#endif


