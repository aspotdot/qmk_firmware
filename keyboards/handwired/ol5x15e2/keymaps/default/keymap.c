
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

#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QT,
    _FN,
    _MO,
    _CM,
    _GM,
    _OH,
    _NM,
};


#define LCTL_BR CTL_T(KC_LBRC)
#define RCTL_BR RCTL_T(KC_RBRC)
#define FN_ENT  LT(_FN,KC_ENT)
#define WN_L    A(G(KC_LEFT))
#define WN_R    A(G(KC_RGHT))
#define WN_MAX  LCAG(KC_UP)
#define WN_SWTC LCAG(KC_LEFT)
#define WN_SHNK A(G(KC_DOWN))
#define WN_INC  A(G(KC_UP))
#define WN_MVL  C(KC_LEFT)
#define WN_MVR  C(KC_RGHT)
#define S_DOWN  S(KC_DOWN)
#define SC_DOWN S(C(KC_DOWN))
#define S_RGHT  S(KC_RGHT)
#define SC_RGHT S(C(KC_RGHT))
#define CA_ESC  C(A(KC_DEL))
#define OH_KC0  LT(_OH,KC_0)
#define NM_DOT  LT(_NM,KC_PDOT)
#define WIN_L   G(KC_LEFT)
#define WIN_R   G(KC_RIGHT)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QT] = LAYOUT_ortho_5x15(
      KC_BTN2, KC_LCTL, KC_LSFT, KC_BTN3,  KC_ESC,  KC_TAB, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, G(KC_SPC), KC_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
      KC_ENT,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_7,    KC_8,    KC_9,    KC_Y,      KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
      KC_TAB,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_4,    KC_5,    KC_6,    KC_H,      KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
      SC_LSPO,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_1,    KC_2,    KC_3,    KC_N,      KC_M, KC_COMM,  KC_DOT, KC_SLSH, SC_RSPC,
      LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC,  OH_KC0,  NM_DOT,  KC_ENT,  KC_SPC,   MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_FN] = LAYOUT_ortho_5x15(
      TG(_GM),  WN_MVL,  WN_MVR, _______,  CA_ESC, _______,  KC_F10,  KC_F11,  KC_F12, _______, _______, _______, DM_RSTP, _______,  AU_TOGG,
      TG(_CM), WN_SHNK,  WN_MAX,  WN_INC, _______, _______,   KC_F7,   KC_F8,   KC_F9, _______, _______, _______, _______, _______, _______,
      KC_PGUP,    WN_L, WN_SWTC,    WN_R, _______, _______,   KC_F4,   KC_F5,   KC_F6, _______, _______, _______, _______, _______, _______,
      KC_PGDN, _______, _______, XXXXXXX, _______, _______,   KC_F1,   KC_F2,   KC_F3, _______, _______, _______, _______, _______, _______,
      _______, XXXXXXX, XXXXXXX, _______, _______,  KC_DEL, XXXXXXX, XXXXXXX, XXXXXXX, KC_UNDS, _______, _______, _______, _______, _______),

    [_MO] = LAYOUT_ortho_5x15(
      _______, KC_BTN1,	_______, _______, _______, _______, _______, _______, _______,	_______, _______, KC_BTN1, DM_REC1,	_______,  _______,
      WIN_L,   WIN_R,   KC_PGDN,   KC_UP, KC_PGUP, _______, _______, KC_UP,   _______,  _______, KC_WH_L, KC_MS_U, KC_WH_R,  KC_WH_U, _______,
      _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, S_RGHT, KC_LEFT, KC_DOWN, KC_RIGHT, _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,  _______,
      _______, _______, SC_DOWN,  S_DOWN, _______, SC_RGHT, _______, _______, _______,  _______, KC_BTN1, XXXXXXX, _______,	_______,  _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______,	_______, _______),

    [_CM] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G, _______, _______, _______,    KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, _______,
      _______,    KC_A,    KC_R,    KC_S,    KC_T,    KC_D, _______, _______, _______,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O, _______,
      _______,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, _______, _______, _______,    KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_GM] = LAYOUT_ortho_5x15(
      TO(_QT),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       KC_ESC,    KC_G,    KC_Q,    KC_W,    KC_E,    KC_R, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       KC_TAB,    KC_M,    KC_A,    KC_S,    KC_D,    KC_F, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_LSFT,    KC_I,    KC_Z,    KC_X,    KC_C,    KC_V, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_LCTL,    KC_K, XXXXXXX, XXXXXXX, KC_BTN2,  KC_SPC, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_OH] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_BSLS,    KC_P,    KC_O,    KC_I,    KC_U,    KC_Y, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      KC_QUOT, KC_SCLN,    KC_L,    KC_K,    KC_J,    KC_H, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, KC_SLSH,  KC_DOT, KC_COMM,    KC_M,    KC_N, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______,  KC_SPC, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NM] = LAYOUT_ortho_5x15(
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,  KC_EQL, _______, _______,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
      _______,  KC_GRV,   KC_LT,   KC_GT, KC_DQUO,  KC_DOT, KC_AMPR, KC_ASTR, KC_LPRN, KC_AMPR, XXXXXXX, KC_LBRC, KC_RBRC, KC_PERC, _______,
      _______, KC_EXLM, KC_MINS, KC_PLUS,  KC_EQL, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN,   KC_AT, _______,
      _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, KC_TILD,  KC_DLR, KC_LCBR, KC_RCBR, KC_QUES, _______,
      _______, _______, _______, _______, _______, _______, KC_RPRN, _______,  KC_EQL, _______, _______, _______, _______, _______, _______)
  };


// key overrides
const key_override_t space_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SPC, KC_UNDS);
const key_override_t enter_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_ENT, KC_EQL);
const key_override_t plus_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_EQL);
// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &space_key_override,
    &enter_key_override,
    &plus_key_override,
    NULL // Null terminate the array of overrides!
  };



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QT] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_FN] =  { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),   ENCODER_CCW_CW(KC_PGUP, KC_PGDN)  },
    [_MO] =  { ENCODER_CCW_CW(KC_WH_D, KC_WH_U),    ENCODER_CCW_CW(KC_WH_R, KC_WH_L)  },
    [_CM] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_GM] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_OH] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_NM] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
};
#endif

#ifdef OLED_ENABLE

// base icons
 static const char   top[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0};
 static const char  eyes[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0};
 static const char mouth[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};

 static const char   GLtop[] = {0x94, 0x95, 0x96, 0x97, 0x98, 0};
 static const char  GLeyes[] = {0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0};
 static const char GLmouth[] = {0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0};


// icon options
 static const char mindblown[] = {0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0}; // top
 static const char       cat[] = {0x85, 0x86, 0x87, 0x88, 0x89, 0}; // top
 static const char      pong[] = {0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0}; // eyes
 static const char mouthOpen[] = {0xaF, 0xb0, 0xb1, 0xb2, 0xb3, 0}; // mouth
 static const char   vampire[] = {0xcF, 0xd0, 0xd1, 0xd2, 0xd3, 0}; // mouth

 static  uint32_t sleep_timer = 0;
 static  uint32_t glitch_timer = 0;
 const uint8_t single_bit_masks[8] = {127, 191, 223, 239, 247, 251, 253, 254}; //Setup some mask which can be or'd with bytes to turn off pixels


 void drawscreen(void) {
  // draw top
  oled_set_cursor(8,1);
  switch (get_highest_layer(layer_state)) {
    case _MO:
      oled_write(mindblown,false);
      break;
    case _OH:
      oled_write(cat,false);
      break;
    default:
      oled_write(top,false);
    }
  // draw eyes
  oled_set_cursor(8,2);
  switch (get_highest_layer(layer_state)) {
    case _GM:
      oled_write(pong,false);
      break;
    default:
      if (get_current_wpm() > 50) {
        char wpm_str[4];
        uint8_t n = get_current_wpm();
        wpm_str[3] = '\0';
        wpm_str[2] = '0' + n % 10;
        wpm_str[1] = '0' + (n /= 10) % 10;
        wpm_str[0] = '0' + n / 10 ;
        oled_advance_char();
        oled_write(wpm_str, false);
       } else {
         oled_write(eyes,false);
       }
     }
  // draw mouth
  oled_set_cursor(8,3);
    switch (get_highest_layer(layer_state)) {
      case _FN:
        oled_write(mouthOpen,false);
        break;
      case _NM:
        oled_write(vampire,false);
        break;
      default:
        oled_write(mouth,false);
      }

    if (get_mods() & MOD_MASK_ALT) {
      oled_set_cursor(9, 2);
      oled_write_char(0x9a, false);
    }
    if (get_mods() & MOD_MASK_GUI) {
      oled_set_cursor(9, 2);
      oled_write_char(0x99, get_mods() & MOD_MASK_ALT);
    }
    if (get_mods() & MOD_MASK_CTRL) {
      oled_set_cursor(11, 2);
      oled_write_char(0x9b, false);
    }
    if (get_mods() & MOD_MASK_SHIFT) {
      oled_set_cursor(11, 2);
      oled_write_char(0x9c, get_mods() & MOD_MASK_CTRL);
    }
 }


  void screen_save(void) {
       if (timer_elapsed32(glitch_timer) > 50) { //rand()%(200-30+1)+30
         glitch_timer = timer_read32();

            char  eyeL = rand() % 223;

         uint8_t GLpick = rand()%(6-1+1)+1;  //rand() % (ub - lb + 1)) + lb
           switch (GLpick) {
             case 1:
              oled_set_cursor(8,1);
              oled_write(GLtop,false);
             break;
             case 2:
              oled_set_cursor(8,1);
              oled_write(top,false);
             break;
             case 3:
              oled_set_cursor(8,2);
              oled_write(eyeL,false);
             break;
             case 4:
              oled_set_cursor(8,2);
              oled_write(eyes,false);
             break;
             case 5:
              oled_set_cursor(8,3);
              oled_write(GLmouth,false);
             break;
             case 6:
              oled_set_cursor(8,3);
              oled_write(mouth,false);
             break;
             }
           }
         }


  bool oled_task_user(void) {
    if(get_current_wpm() != 000) {
        oled_on(); // not essential but turns on animation OLED with any alpha keypress
        drawscreen();
    } else {
      if (timer_elapsed32(sleep_timer) > 30000) {
         screen_save();
       }
     }
       return false;
     }


#endif  //end oled








