/*
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

qmk lint -kb handwired/ol5x15e2
qmk compile -kb handwired/ol5x15e2 -km default
 */

#include QMK_KEYBOARD_H


#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _00,
    _QT,
    _CM,
    _CN,
    _FN,
    _MO,
    _NM,
    _GM,
};



#define LCTL_BR CTL_T(KC_LBRC)
#define RCTL_BR RCTL_T(KC_RBRC)

#define S_DOWN  S(KC_DOWN)
#define SC_DOWN S(C(KC_DOWN))
#define S_RGHT  S(KC_RGHT)
#define SC_RGHT S(C(KC_RGHT))
#define CA_ESC  C(A(KC_DEL))

#define NM_DOT  LT(_NM,KC_PDOT)

#define WIN_L   G(KC_LEFT)
#define WIN_R   G(KC_RIGHT)
#define WN_MAX  G(KC_UP)
#define WN_FLL  G(S(KC_UP))
#define WN_MON  G(S(KC_LEFT))



enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  SRCHSEL,
  KY_DEG,
  KY_DIA,
  KY_PLMN,
  JIGGLE,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_00] = LAYOUT_ortho_5x15(
      KC_BTN2, KC_LCTL, KC_LSFT, KC_BTN3,  KC_ESC,  KC_TAB,    KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR,  KC_PSCR, KC_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
      KC_ENT,  _______, _______, _______, _______, _______,      KC_P7,   KC_P8,   KC_P9, _______,  _______, _______, _______, _______, _______,
      KC_TAB,  _______, _______, _______, _______, _______,      KC_P4,   KC_P5,   KC_P6, _______,  _______, _______, _______, _______, _______,
      SC_LSPO, _______, _______, _______, _______, _______,      KC_P1,   KC_P2,   KC_P3, _______,  _______, _______, _______, _______, SC_RSPC,
      LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC, SH_T(KC_P0), NM_DOT, KC_PENT,  KC_SPC,  MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_QT] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
      _______,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, _______, _______, _______,    KC_Y,     KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
      _______,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G, _______, _______, _______,    KC_H,     KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
      _______,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, _______, _______, _______,    KC_N,     KC_M, KC_COMM,  KC_DOT, KC_SLSH, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______),

    [_CM] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B, _______, _______, _______,    KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT, _______,
      _______,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G, _______, _______, _______,    KC_K,    KC_N,    KC_E,    KC_I,    KC_A, _______,
      _______,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V, _______, _______, _______,    KC_M,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_CN] = LAYOUT_ortho_5x15(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______,    KC_W,    KC_L,    KC_Y,    KC_P,    KC_B, _______, _______, _______,    KC_Z,    KC_F,    KC_O,    KC_U, KC_SCLN, _______,
      _______,    KC_C,    KC_R,    KC_S,    KC_T,    KC_G, _______, _______, _______,    KC_M,    KC_N,    KC_E,    KC_I,    KC_A, _______,
      _______,    KC_Q,    KC_J,    KC_V,    KC_D,    KC_K, _______, _______, _______,    KC_X,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_FN] = LAYOUT_ortho_5x15(
      TO(_GM), JIGGLE, _______, _______,  CA_ESC, _______,  KC_F10,  KC_F11,  KC_F12, _______, _______, _______, _______, _______,  AU_TOGG,
      SRCHSEL, _______, _______, _______, _______, _______,   KC_F7,   KC_F8,   KC_F9, _______, _______, _______, _______, _______,  TO(_QT),
      _______, _______, _______, _______, _______, _______,   KC_F4,   KC_F5,   KC_F6, _______, _______, _______, _______, _______,  TO(_CM),
      SELWORD, C(KC_X), C(KC_C), C(KC_V), _______, _______,   KC_F1,   KC_F2,   KC_F3, _______, _______, _______, _______, _______,  TO(_CN),
      _______, _______, _______, _______, _______,  KC_DEL, XXXXXXX, XXXXXXX, XXXXXXX, KC_UNDS, _______, _______, _______, _______, _______),

    [_MO] = LAYOUT_ortho_5x15(
      _______, _______,	_______, KC_BTN1, _______, _______, _______, _______, _______,	_______, _______, KC_BTN1, DM_REC1,	_______,  _______,
        WIN_L,   WIN_R, KC_PGDN,   KC_UP, KC_PGUP, _______, _______,   KC_UP, _______,  _______, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U,  _______,
       WN_FLL, _______, KC_LEFT, KC_DOWN, KC_RIGHT, S_RGHT, KC_LEFT, KC_DOWN, KC_RIGHT, _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D,  _______,
       WN_MON, _______, SC_DOWN,  S_DOWN, XXXXXXX, SC_RGHT, _______, _______, _______,  _______, KC_BTN1, XXXXXXX, _______,	_______,  _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______,	_______,  _______),

    [_NM] = LAYOUT_ortho_5x15(
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, KY_PLMN,  KC_NUM,  KY_DEG,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,
      _______,  KC_GRV,   KC_LT,   KC_GT, KC_DQUO,  KC_DOT, KC_AMPR, KC_ASTR, KC_LPRN, KC_AMPR,  KY_DIA, KC_LBRC, KC_RBRC, KC_PERC, _______,
      _______, KC_EXLM, KC_MINS, KC_PLUS,  KC_EQL, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN,   KC_AT, _______,
      _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, KC_TILD,  KC_DLR, KC_LCBR, KC_RCBR, KC_QUES, _______,
      _______, _______, _______, _______, _______, _______, KC_RPRN, _______,  KC_EQL, _______, _______, _______, _______, _______, _______),

    [_GM] = LAYOUT_ortho_5x15(
      TO(_00),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       KC_ESC,    KC_G,    KC_Q,    KC_W,    KC_E,    KC_R,   KC_P7,   KC_P8,   KC_P9, _______, _______, _______, _______, _______, _______,
       KC_TAB,    KC_M,    KC_A,    KC_S,    KC_D,    KC_F,   KC_P4,   KC_P5,   KC_P6, _______, _______, _______, _______, _______, _______,
      KC_LSFT,    KC_I,    KC_Z,    KC_X,    KC_C,    KC_V,   KC_P1,   KC_P2,   KC_P3, _______, _______, _______, _______, _______, _______,
      KC_LCTL,    KC_K, XXXXXXX, XXXXXXX, KC_BTN2,  KC_SPC,   KC_P0, _______, _______, _______, _______, _______, _______, _______, _______)

  };


/*
// key overrides
#ifdef KEY_OVERRIDE_ENABLE
const key_override_t space_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SPC, KC_UNDS);
const key_override_t enter_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PENT, KC_EQL);
const key_override_t plus_key_override =  ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_EQL);
// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &space_key_override,
    &enter_key_override,
    &plus_key_override,
    NULL // Null terminate the array of overrides!
  };
#endif
*/



// Mouse jiggler: add JIGGLE macro and keycode
// https://www.reddit.com/r/olkb/comments/t4imri/comment/hz2w67i/?context=3
bool mouse_jiggler_enabled = false;
static uint16_t mouse_jiggler_timer;

bool has_mouse_report_changed(report_mouse_t* new_report, report_mouse_t* old_report) {
  // Only report every 5 seconds.
  if (mouse_jiggler_enabled && timer_elapsed(mouse_jiggler_timer) > 5000) {
    mouse_jiggler_timer = timer_read();
    return mouse_jiggler_enabled;
  }
  return memcmp(new_report, old_report, sizeof(report_mouse_t));
}
void mouse_jiggle_toggle(void) {
  mouse_jiggler_timer = timer_read();
  mouse_jiggler_enabled = ! mouse_jiggler_enabled;
}

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
//    case JIGGLE:
//          mouse_jiggle_toggle();
//          return false;

    }
  return true;
}






#ifdef SWAP_HANDS_ENABLE

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] =
    // The LAYOUT macro could work for this, but it was harder to figure out the
    // identity using it.

    // This is the identity layout.
/*
{
    { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, },
    { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {10, 1}, {11, 1}, {12, 1}, {13, 1}, {14, 1}, },
    { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}, {12, 2}, {13, 2}, {14, 2}, },
    { {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {11, 3}, {12, 3}, {13, 3}, {14, 3}, },
    { {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4}, {10, 4}, {11, 4}, {12, 4}, {13, 4}, {14, 4}, },

};
*/

    // This is the mirror, letter keys and space are mirrored
{
    // enc      1       2        3        4        5     |        num pad        | 9       10       11       12       13       enc       enc
    { {0, 0}, {1, 0},  {2, 0},  {3, 0},  {4, 0},  {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}, {14, 0} },
    { {0, 1}, {13, 1}, {12, 1}, {11, 1}, {10, 1}, {9, 1}, {6, 1}, {7, 1}, {8, 1}, {5, 1}, {4, 1},  {3, 1},  {2, 1},  {1, 1},  {14, 1} },
    { {0, 2}, {13, 2}, {12, 2}, {11, 2}, {10, 2}, {9, 2}, {6, 2}, {7, 2}, {8, 2}, {5, 2}, {4, 2},  {3, 2},  {2, 2},  {1, 2},  {14, 2} },
    { {0, 3}, {13, 3}, {12, 3}, {11, 3}, {10, 3}, {9, 3}, {6, 3}, {7, 3}, {8, 3}, {5, 3}, {4, 3},  {3, 3},  {2, 3},  {1, 3},  {14, 3} },
    { {0, 4}, {1, 4},  {2, 4},  {3, 4},  {4, 4},  {9, 4}, {6, 4}, {7, 4}, {8, 4}, {5, 4}, {10, 4}, {11, 4}, {12, 4}, {13, 4}, {14, 4} },
};


#ifdef ENCODER_MAP_ENABLE
    const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {0};
#endif

#endif



#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_00] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_QT] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_FN] =  { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),   ENCODER_CCW_CW(KC_PGUP, KC_PGDN)  },
    [_MO] =  { ENCODER_CCW_CW(KC_WH_D, KC_WH_U),    ENCODER_CCW_CW(KC_WH_R, KC_WH_L)  },
    [_CM] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_CN] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_GM] =  { ENCODER_CCW_CW(KC_UP, KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
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
  static void fade_display(void) {
      //Define the reader structure
      oled_buffer_reader_t reader;
      uint8_t buff_char;
      if (random() % 30 == 0) {
          srand(timer_read());
          // Fetch a pointer for the buffer byte at index 0. The return structure
          // will have the pointer and the number of bytes remaining from this
          // index position if we want to perform a sequential read by
          // incrementing the buffer pointer
          reader = oled_read_raw(0);
          //Loop over the remaining buffer and erase pixels as we go
          for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
              //Get the actual byte in the buffer by dereferencing the pointer
              buff_char = *reader.current_element;
              if (buff_char != 0) {
                  oled_write_raw_byte(buff_char & single_bit_masks[rand() % 8], i);
              }
              //increment the pointer to fetch a new byte during the next loop
              reader.current_element++;
          }
      }
  }




 void drawscreen(void) {
    switch (get_highest_layer(layer_state)) {
    case _00:
      oled_write_P(PSTR("Default\n"), false);
      break;
    case _QT:
      oled_write_P(PSTR("qwerty\n"), false);
      break;
    case _CM:
      oled_write_P(PSTR("colmack\n"), false);
      break;
    case _CN:
      oled_write_P(PSTR("canary\n"), false);
      break;
    case _FN:
      oled_write_P(PSTR("function\n"), false);
      break;
    case _MO:
      oled_write_P(PSTR("move\n"), false);
      break;
    case _GM:
      oled_write_P(PSTR("game\n"), false);
      break;
    case _NM:
      oled_write_P(PSTR("number\n"), false);
      break;
    default:
        oled_write_P(PSTR("**\n"), false);     }



  // draw top
  oled_set_cursor(8,1);
  switch (get_highest_layer(layer_state)) {
    case _MO:
      oled_write(mindblown,false);
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
         oled_write(eyes,false);
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
       if (timer_elapsed32(glitch_timer) > 100) { //rand()%(200-30+1)+30
         glitch_timer = timer_read32();

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
              oled_write(GLeyes,false);
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
      } else if (timer_elapsed32(sleep_timer) > 30000 && timer_elapsed32(sleep_timer) < 90000) {
           screen_save();
      } else {
         fade_display();
      }
       return false;
     }


#endif  //end oled

