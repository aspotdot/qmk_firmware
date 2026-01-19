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
qmk flash -kb handwired/ol5x15e2 -km default

 */


#include QMK_KEYBOARD_H
#include "oled.h"
#include "rpn_calc.h"


#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif


// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QT,
    _FN,
    _MO,
    _NM,
    _CM,
    _CN,
    _GM,
    _CL
};

// Custom keycodes
#define LCTL_BR CTL_T(KC_LBRC)
#define RCTL_BR RCTL_T(KC_RBRC)

#define S_UP    S(KC_UP)
#define SC_UP   S(C(KC_UP))
#define S_DOWN  S(KC_DOWN)
#define SC_DOWN S(C(KC_DOWN))
#define S_RGHT  S(KC_RGHT)
#define SC_RGHT S(C(KC_RGHT))
#define S_LEFT  S(KC_LEFT)
#define SC_LEFT S(C(KC_LEFT))

#define CA_ESC   C(A(KC_DEL))
#define SH_0     SH_T(KC_0)
#define NM_DOT   LT(_NM,KC_PDOT)
#define M1_SFT   LT(KC_LSFT,MS_BTN1)

#define WIN_L   G(KC_LEFT)
#define WIN_R   G(KC_RIGHT)
#define WN_MAX  G(KC_UP)
#define WN_FLL  G(S(KC_UP))
#define WN_MON  G(S(KC_LEFT))


enum custom_keycodes {
  SELWORD = SAFE_RANGE,
  SRCHSEL,
  KY_DEG ,
  KY_DIA ,
  KY_PLMN,
  KY_XLSL,
  JIGGLE ,

  CL_OUT , CL_ENT ,
  CL_CLRS, CL_CLRX, CL_XxY , 
  CL_LOG , CL_LN  ,
  CL_PI  , CL_CE  ,
  CL_SIN , CL_ASIN,
  CL_COS , CL_ACOS,
  CL_TAN , CL_ATAN,
  CL_SQ  , CL_SQRT, CL_XtY ,
  CL_INV , CL_ABS ,
  CL_1   , CL_2   , CL_3   , CL_4   , CL_5   , CL_6   , CL_7   , CL_8   , CL_9   , CL_0   , CL_DOT
};

static deferred_token jiggler_token = INVALID_DEFERRED_TOKEN;
static uint32_t  sleep_timer = 0;



//?


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QT] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, M1_SFT , MS_BTN3, KC_ESC , KC_TAB , KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN2, DM_PLY1, KC_MNXT, KC_MPLY,
    KC_ENT , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_7   , KC_8   , KC_9   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSLS,
    KC_TAB , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_4   , KC_5   , KC_6   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    SC_LSPO, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_1   , KC_2   , KC_3   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC, SH_0   , NM_DOT , KC_PENT, KC_SPC , MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_FN] = LAYOUT_ortho_5x15(
    TG(_GM),  JIGGLE, _______, _______,  CA_ESC, _______, KC_F10 ,  KC_F11,  KC_F12, _______, _______, _______, _______, _______, AU_TOGG,
    SRCHSEL, _______,  WN_FLL, _______, _______, _______, KC_F7  ,   KC_F8,   KC_F9, _______, _______, _______, _______, _______,  TO(_QT),
    SELWORD,   WIN_L,  WN_MON,   WIN_R, _______, _______, KC_F4  ,   KC_F5,   KC_F6, _______, _______, _______, _______, _______,  TO(_CM),
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______, KC_F2  ,   KC_F3, _______, _______, _______, _______, _______, _______,  TO(_CN),
    _______, _______, _______, KC_TRNS, _______, _______, _______, _______, _______, KC_UNDS, _______, KC_TRNS, TO(_CL), _______, _______),

    [_MO] = LAYOUT_ortho_5x15(
    _______, _______, _______, SC_UP  , _______, _______, KC_HOME, SC_UP  , KC_END  , _______, _______, MS_BTN1, DM_REC1, _______, _______,
    _______, _______, KC_PGDN, KC_UP  , KC_PGUP, _______, KC_PGDN, KC_UP  , KC_PGUP , _______, MS_WHLL, MS_UP  , MS_WHLR, MS_WHLU, _______,
    _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, S_RGHT, KC_LEFT, KC_DOWN, KC_RIGHT, _______, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD, _______,
    _______, _______, KY_XLSL, S_DOWN , _______, SC_RGHT, S_LEFT , S_DOWN , S_RGHT  , _______, MS_BTN1, _______, _______, _______, _______,
    _______, _______, _______, SC_DOWN, KC_TRNS, _______, SC_LEFT, SC_DOWN, SC_RGHT , _______, KC_TRNS, _______, _______, _______, _______),

    [_NM] = LAYOUT_ortho_5x15(
    _______, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , KY_PLMN, KY_DIA , KY_DEG , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , XXXXXXX,
    _______, KC_GRV , KC_LT  , KC_GT  , KC_DQUO, KC_DOT , KC_AMPR, KC_ASTR, KC_LPRN, KC_AMPR, XXXXXXX, KC_LBRC, KC_RBRC, KC_PERC, XXXXXXX,
    _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN, KC_AT  , XXXXXXX,
    _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, KC_PERC, KC_EXLM, KC_AT  , KC_HASH, KC_TILD, KC_DLR , KC_LCBR, KC_RCBR, KC_QUES, XXXXXXX,
    _______, _______, _______, _______, _______, _______, KC_RPRN, KC_TRNS, KC_EQL , _______, _______, _______, _______, _______, _______),

    [_CM] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, KC_LSFT, MS_BTN3,  KC_ESC,  KC_TAB, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
     KC_ENT, KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   , KC_P7  , KC_P8  , KC_P9  , KC_J   , KC_L   , KC_U   , KC_Y   , KC_QUOT, _______,
     KC_TAB, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   , KC_P4  , KC_P5  , KC_P6  , KC_K   , KC_N   , KC_E   , KC_I   , KC_A   , _______,
    SC_LSPO, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   , KC_P1  , KC_P2  , KC_P3  , KC_M   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC, SH_0   , NM_DOT , KC_PENT , KC_SPC , MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_CN] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, KC_LSFT, MS_BTN3,  KC_ESC,  KC_TAB, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
     KC_ENT, KC_W   , KC_L   , KC_Y   , KC_P   , KC_B   , KC_P7  , KC_P8  , KC_P9  , KC_Z   , KC_F   , KC_O   , KC_Y   , KC_QUOT, _______,
     KC_TAB, KC_C   , KC_R   , KC_S   , KC_T   , KC_G   , KC_P4  , KC_P5  , KC_P6  , KC_M   , KC_N   , KC_E   , KC_I   , KC_A   , _______,
    SC_LSPO, KC_Q   , KC_J   , KC_V   , KC_D   , KC_K   , KC_P1  , KC_P2  , KC_P3  , KC_X   , KC_H   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC, SH_0   , NM_DOT , KC_PENT, KC_SPC , MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_GM] = LAYOUT_ortho_5x15(
    TO(_QT), KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , _______, _______, _______, _______, _______, _______, _______, _______, _______,
    KC_ESC , KC_G   , KC_Q   , KC_W   , KC_E   , KC_R   , KC_P7  , KC_P8  , KC_P9  , _______, _______, _______, _______, _______, _______,
    KC_TAB , KC_M   , KC_A   , KC_S   , KC_D   , KC_F   , KC_P4  , KC_P5  , KC_P6  , _______, _______, _______, _______, _______, _______,
    KC_LSFT, KC_I   , KC_Z   , KC_X   , KC_C   , KC_V   , KC_P1  , KC_P2  , KC_P3  , _______, _______, _______, _______, _______, _______,
    KC_LCTL, KC_K   , XXXXXXX, XXXXXXX, MS_BTN2, KC_SPC , KC_P0  , _______, _______, _______, _______, _______, _______, _______, _______),

    [_CL] = LAYOUT_ortho_5x15(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, CL_CLRS, KC_PPLS, KC_PMNS, KC_PSLS, KC_PAST, CL_INV , XXXXXXX, XXXXXXX, _______, _______,
    TO(_QT), XXXXXXX, CL_SIN , CL_ASIN, CL_LN  , CL_CLRX, CL_7   , CL_8   , CL_9   , KC_CIRC, CL_PI  , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, CL_COS , CL_ACOS, CL_LOG , CL_XxY , CL_4   , CL_5   , CL_6   , CL_SQRT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, CL_TAN , CL_ATAN, CL_ABS , CL_INV , CL_1   , CL_2   , CL_3   , CL_SQ  , CL_XtY , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, CL_OUT , CL_0   , CL_DOT , CL_ENT , CL_OUT , KC_PENT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX)

  };
 
 
 
 
 
 
  // Need to evaluate if COMBO is  useful
#ifdef COMBO_ENABLE
    const uint16_t PROGMEM cmb_up[] = {KC_UP,  KC_PGUP, COMBO_END};
    const uint16_t PROGMEM cmb_dn[] = {KC_UP,  KC_PGDN, COMBO_END};
    const uint16_t PROGMEM cmb_ar[] = {KC_PENT, KC_SPC, COMBO_END};

    combo_t key_combos[] = {
        COMBO(cmb_up, KC_HOME),
        COMBO(cmb_dn, KC_END),
        COMBO(cmb_ar, QK_AREP)
    };
#endif

#ifdef KEY_OVERRIDE_ENABLE
    const key_override_t space_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_SPC, KC_UNDS);
    const key_override_t enter_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_PENT, KC_EQL);
    const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

    const key_override_t *key_overrides[] = {
        &space_key_override,
        &enter_key_override,
        &delete_key_override
    };
#endif

#ifdef ENCODER_MAP_ENABLE
    const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {0};

    const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
        [_QT] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_FN] =  { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),      ENCODER_CCW_CW(KC_PGUP, KC_PGDN)  },
        [_MO] =  { ENCODER_CCW_CW(MS_WHLU , MS_WHLD),      ENCODER_CCW_CW(MS_WHLR, MS_WHLL)  },
        [_NM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_BRID, KC_BRIU)  },
        [_CM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_CN] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_GM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_CL] = { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  }
    };
#endif




// Jiggler Callback (File Scope)
static bool jiggler_enable = false;
static uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
    static report_mouse_t report = {0};
    // Deltas to move in a circle of radius 20 pixels over 32 frames.
    static const int8_t deltas[32] = {
        0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
        0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
    static uint8_t phase = 0;
    // Get x delta from table and y delta by rotating a quarter cycle.
    report.x = deltas[phase];
    report.y = deltas[(phase + 8) & 31];
    phase = (phase + 1) & 31;
    host_mouse_send(&report);
    return 16;  // Call the callback every 16 ms.
}

bool is_jiggler_active(void) {
    return jiggler_token != INVALID_DEFERRED_TOKEN;
}

bool is_jiggler_enabled(void) {
    return jiggler_enable;
}


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        sleep_timer = timer_read32();
        reset_oled_timer();
        // Stop jiggling on any keypress (wake up)
        if (jiggler_token) {
            cancel_deferred_exec(jiggler_token);
            jiggler_token = INVALID_DEFERRED_TOKEN;
            report_mouse_t report = {0};
            host_mouse_send(&report);
            oled_clear();
        }
        #ifdef OLED_ENABLE
        trigger_matrix_rain(keycode);
        #endif
    }

  // Macro set up: ref //https://getreuer.info/posts/keyboards/macros/index.html
  switch (keycode) {
    case SELWORD:  // Selects the current word under the cursor.
        if (record->event.pressed) {
            SEND_STRING(SS_LCTL(SS_TAP(X_RGHT) SS_LSFT(SS_TAP(X_LEFT))));
        }
            return false;
    case SRCHSEL:  // Searches the current selection in a new tab.
        if (record->event.pressed) {
            SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
        }
            return false;
    case KY_DEG:  // types degree.
        if (record->event.pressed) {
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_7) SS_TAP(X_KP_6)));
        }
            return false;
    case KY_PLMN:  // types plus minus .
        if (record->event.pressed) {
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_7) SS_TAP(X_KP_7)));
        }
            return false;
    case KY_DIA:  // diameter.
        if (record->event.pressed) {
            SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_2) SS_TAP(X_KP_4) SS_TAP(X_KP_8)));
        }
            return false;
    case KY_XLSL:  // Select range in XL.
        if (record->event.pressed) {
            SEND_STRING(SS_LCTL(SS_TAP(X_UP) SS_TAP(X_LEFT)) SS_LSFT(SS_LCTL(SS_TAP(X_DOWN) SS_TAP(X_RIGHT))));
        }
            return false;
    case JIGGLE:
        if (record->event.pressed) {
            jiggler_enable = !jiggler_enable;
        }
        return false;
    }

    // End standard macros

  // Handle Calculator Mode
  if (get_highest_layer(layer_state) == _CL) {
      if (keycode == TO(_QT)) {
          if (record->event.pressed) {
              layer_off(_CL);
              layer_on(_QT);
          }
          return false; 
      }
      if (keycode == TO(_FN)) return true; // Allow standard switch to FN if needed
      if (keycode == CL_OUT) {
          if (record->event.pressed) {
              calc_output_result();
          }
          return false;
      }
            // Pass to calculator engine
      if (record->event.pressed) {
          if (calc_handle_key(keycode)) {
              return false; // Handled by calc engine, don't send key to host
          }
      } else {
          // Ignore releases for calc keys to prevent double inputs if engine handled press
          // But we need to be careful. calc_handle_key only returns true if handled.
          // If we return false here on release, QMK processes it. 
          // Since calc_handle_key is only called on press, we might need to block release too if it's a calc key?
          // For simplicity, let's block standard calc keys releases if they were handled.
          // Actually, best is to block all keys except TO().
          return false; 
      }
      return false; // Block everything else in this layer unless handled above
  }



  return true;
}


void matrix_scan_user(void) {
    if (jiggler_enable && jiggler_token == INVALID_DEFERRED_TOKEN) {
        if (timer_elapsed32(sleep_timer) > 2*60*1000) { // 2 minutes 
             jiggler_token = defer_exec(1, jiggler_callback, NULL);
             //init_matrixAni(); // handled in oled.c
        }
    }
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

#endif
