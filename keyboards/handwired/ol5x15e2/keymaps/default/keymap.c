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
    _GM
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
  KY_DEG,
  KY_DIA,
  KY_PLMN,
  KY_XLSL,
  JIGGLE
};

static deferred_token jiggler_token = INVALID_DEFERRED_TOKEN;
static uint32_t sleep_timer = 0;
static uint32_t glitch_timer = 0;

#ifdef OLED_ENABLE
void init_matrixAni(void);
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QT] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, M1_SFT , MS_BTN3, KC_ESC,  KC_TAB , KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN2, DM_PLY1, KC_MNXT, KC_MPLY,
    KC_ENT , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   , KC_7   , KC_8   , KC_9   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSLS,
    KC_TAB , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , KC_4   , KC_5   , KC_6   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
    SC_LSPO, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_1   , KC_2   , KC_3   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC, SH_0   , NM_DOT , KC_PENT, KC_SPC , MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_FN] = LAYOUT_ortho_5x15(
    TG(_GM),  JIGGLE, _______, _______,  CA_ESC, _______,  KC_F10,  KC_F11,  KC_F12, _______, _______, _______, _______, _______, AU_TOGG,
    SRCHSEL, _______,  WN_FLL, _______, _______, _______,   KC_F7,   KC_F8,   KC_F9, _______, _______, _______, _______, _______,  TO(_QT),
    SELWORD,   WIN_L,  WN_MON,   WIN_R, _______, _______,   KC_F4,   KC_F5,   KC_F6, _______, _______, _______, _______, _______,  TO(_CM),
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), _______,   KC_F2,   KC_F3, _______, _______, _______, _______, _______, _______,  TO(_CN),
    _______, _______, _______, KC_TRNS, _______,  KC_DEL, _______, _______, _______, KC_UNDS, _______, KC_TRNS, _______, _______, _______),

    [_MO] = LAYOUT_ortho_5x15(
    _______, _______, _______,   SC_UP, _______, _______, KC_HOME,   SC_UP,   KC_END, _______, _______, MS_BTN1, DM_REC1, _______, _______,
    _______, _______, KC_PGDN,   KC_UP, KC_PGUP, _______, KC_PGDN,   KC_UP,  KC_PGUP, _______, MS_WHLL, MS_UP  , MS_WHLR, MS_WHLU, _______,
    _______, _______, KC_LEFT, KC_DOWN, KC_RIGHT, S_RGHT, KC_LEFT, KC_DOWN, KC_RIGHT, _______, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD, _______,
    _______, _______, KY_XLSL,  S_DOWN, _______, SC_RGHT,  S_LEFT,  S_DOWN,   S_RGHT, _______, MS_BTN1, _______, _______, _______, _______,
    _______, _______, _______, SC_DOWN, KC_TRNS, _______, SC_LEFT, SC_DOWN,  SC_RGHT, _______, KC_TRNS, _______, _______, _______, _______),

    [_NM] = LAYOUT_ortho_5x15(
    _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, KY_PLMN,  KY_DIA,  KY_DEG,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0, XXXXXXX,
    _______,  KC_GRV,   KC_LT,   KC_GT, KC_DQUO,  KC_DOT, KC_AMPR, KC_ASTR, KC_LPRN, KC_AMPR,   KC_NO, KC_LBRC, KC_RBRC, KC_PERC, XXXXXXX,
    _______, KC_EXLM, KC_MINS, KC_PLUS,  KC_EQL, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_PIPE, KC_COLN, KC_LPRN, KC_RPRN,   KC_AT, XXXXXXX,
    _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, XXXXXXX, KC_EXLM,   KC_AT, KC_HASH, KC_TILD,  KC_DLR, KC_LCBR, KC_RCBR, KC_QUES, XXXXXXX,
    _______, _______, _______, _______, _______, _______, KC_RPRN,  KC_TRNS, KC_EQL, _______, _______, _______, _______, _______, _______),

    [_CM] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, KC_LSFT, MS_BTN3,  KC_ESC,  KC_TAB, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
     KC_ENT,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,   KC_P7,   KC_P8,   KC_P9,    KC_J,    KC_L,    KC_U,    KC_Y, KC_QUOT, _______,
     KC_TAB,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,   KC_P4,   KC_P5,   KC_P6,    KC_K,    KC_N,    KC_E,    KC_I,    KC_A, _______,
    SC_LSPO,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,   KC_P1,   KC_P2,   KC_P3,    KC_M,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC,   SH_0,  NM_DOT, KC_PENT,  KC_SPC, MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_CN] = LAYOUT_ortho_5x15(
    MS_BTN2, KC_LCTL, KC_LSFT, MS_BTN3,  KC_ESC,  KC_TAB, KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PSCR, MS_BTN1, DM_PLY1, KC_MNXT, KC_MPLY,
     KC_ENT,    KC_W,    KC_L,    KC_Y,    KC_P,    KC_B,   KC_P7,   KC_P8,   KC_P9,    KC_Z,    KC_F,    KC_O,    KC_Y, KC_QUOT, _______,
     KC_TAB,    KC_C,    KC_R,    KC_S,    KC_T,    KC_G,   KC_P4,   KC_P5,   KC_P6,    KC_M,    KC_N,    KC_E,    KC_I,    KC_A, _______,
    SC_LSPO,    KC_Q,    KC_J,    KC_V,    KC_D,    KC_K,   KC_P1,   KC_P2,   KC_P3,    KC_X,    KC_H, KC_COMM,  KC_DOT, KC_SLSH, SC_RSPC,
    LCTL_BR, KC_LALT, KC_LGUI, MO(_FN), MO(_MO), KC_BSPC,   SH_0,  NM_DOT, KC_PENT,  KC_SPC, MO(_MO), MO(_FN), KC_RGUI, KC_RALT, RCTL_BR),

    [_GM] = LAYOUT_ortho_5x15(
    TO(_QT),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5, _______, _______, _______, _______, _______, _______, _______, _______, _______,
     KC_ESC,    KC_G,    KC_Q,    KC_W,    KC_E,    KC_R,   KC_P7,   KC_P8,   KC_P9, _______, _______, _______, _______, _______, _______,
     KC_TAB,    KC_M,    KC_A,    KC_S,    KC_D,    KC_F,   KC_P4,   KC_P5,   KC_P6, _______, _______, _______, _______, _______, _______,
    KC_LSFT,    KC_I,    KC_Z,    KC_X,    KC_C,    KC_V,   KC_P1,   KC_P2,   KC_P3, _______, _______, _______, _______, _______, _______,
    KC_LCTL,    KC_K, XXXXXXX, XXXXXXX, MS_BTN2,  KC_SPC,   KC_P0, _______, _______, _______, _______, _______, _______, _______, _______)

  };

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
    const key_override_t plus_key_override =  ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_EQL);
    const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

    const key_override_t *key_overrides[] = {
        &space_key_override,
        &enter_key_override,
        &plus_key_override,
        &delete_key_override
    };
#endif


// Jiggler Callback (File Scope)
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

static bool jiggler_enable = false;

// Matrix Rain Implementation
#ifdef OLED_ENABLE
    #define DRAINS_COLS 21 // 128 pixels / 6px font width = 21.3
    #define DRAINS_ROWS 4  // 32 pixels / 8px font height
    // 127 will represent "inactive" or "off screen" safely above DRAINS_ROWS
    static int8_t matrix_drops[DRAINS_COLS];
    static uint8_t matrix_speeds[DRAINS_COLS];
    static char matrix_col_chars[DRAINS_COLS];

    void init_matrixAni(void) {
        for (int i = 0; i < DRAINS_COLS; i++) {
            matrix_drops[i] = 127; // Start inactive
            matrix_col_chars[i] = 0x01 + (rand() % (0xDF - 0x01)); 
        }
    }

    static void update_matrix(void) {
        static uint8_t tick = 0;
        tick++;
        for (int i = 0; i < DRAINS_COLS; i++) {
            // Only advance if active (not 127)
            if (matrix_drops[i] != 127) {
                 // Advance drop based on speed
                if (tick % matrix_speeds[i] == 0) {
                    matrix_drops[i]++;
                }
            }
        
        // Reset if it fell off screen (height + tail length)
        // OR if it's inactive (127) and Jiggler is ON, we might want to start it?
        // Actually, init_matrixAni handles the start. Here we just handle the loop.
        
        if (matrix_drops[i] > DRAINS_ROWS + 4) {
             if (jiggler_token != INVALID_DEFERRED_TOKEN) {
                 // Loop if Jiggler is ON
                 matrix_drops[i] = -(rand() % 10);
                 matrix_speeds[i] = (rand() % 4) + 1;
                 matrix_col_chars[i] = 0x01 + (rand() % (0xDF - 0x01));
             } else {
                 // Stop if Jiggler is OFF (Typing mode ends drop)
                 matrix_drops[i] = 127; 
             }
        }
        
        // If Jiggler is ON and drop is inactive, chance to start it
        if (jiggler_token != INVALID_DEFERRED_TOKEN && matrix_drops[i] == 127) {
            if ((rand() % 100) < 5) { // 5% chance to start a drop per frame
                matrix_drops[i] = -(rand() % 10);
                 matrix_speeds[i] = (rand() % 4) + 1;
                 matrix_col_chars[i] = 0x01 + (rand() % (0xDF - 0x01));
            }
        }
    }
}

static void draw_matrix(void) {
    for (int col = 0; col < DRAINS_COLS; col++) {
        int head = matrix_drops[col];
        int x = col * 6;
        
        if (head >= 0 && head < DRAINS_ROWS) {
            oled_set_cursor(x / 6, head); // Column is in chars, Row is in chars
            // Check if char is valid (not 0), else random (shouldnt happen)
            unsigned char r = matrix_col_chars[col];
            if (r == 0) r = 0x01 + (rand() % (0xDF - 0x01));
            oled_write_char(r, false);
        }
        
        // Randomize the character immediately behind the head to create a random trail
        int tail_start = head - 1;
        if (tail_start >= 0 && tail_start < DRAINS_ROWS) {
            oled_set_cursor(x / 6, tail_start);
            unsigned char rand_char = 0x01 + (rand() % (0xDF - 0x01));
            oled_write_char(rand_char, false);
        }

        // Erase trail (very simple single-falling-char effect for now, 
        // essentially the "drop" is length 1. To make it a trail, we check positions above head)
        // Let's make a trail of length 4
        int tail_end = head - 4;
        if (tail_end >= 0 && tail_end < DRAINS_ROWS) {
             oled_set_cursor(x / 6, tail_end);
             oled_write_char(' ', false);
        }
    }
}
#endif



void keyboard_post_init_user(void) {
    #ifdef OLED_ENABLE
    init_matrixAni();
    #endif
}

// Macro set up: ref //https://getreuer.info/posts/keyboards/macros/index.html
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (record->event.pressed) {
    sleep_timer = timer_read32();
    // Stop jiggling on any keypress (wake up)
    if (jiggler_token) {
        cancel_deferred_exec(jiggler_token);
        jiggler_token = INVALID_DEFERRED_TOKEN;
        report_mouse_t report = {0};
        host_mouse_send(&report);
        oled_clear();
        init_matrixAni(); // Reset matrix to inactive state
    }
    
    // Typing rain trigger
    #ifdef OLED_ENABLE
    char charDrop = 0;
    if (keycode >= KC_A && keycode <= KC_Z) charDrop = 'a' + (keycode - KC_A);
    else if (keycode >= KC_1 && keycode <= KC_0) charDrop = (keycode == KC_0) ? '0' : '1' + (keycode - KC_1);
    
    if (charDrop) {
        int col = rand() % DRAINS_COLS;
        matrix_drops[col] = 0; // Start at top
        matrix_col_chars[col] = charDrop;
        matrix_speeds[col] = 2; // Fast drop for typing 
    }
    #endif
  }
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

  return true;
}

void matrix_scan_user(void) {
    if (jiggler_enable && jiggler_token == INVALID_DEFERRED_TOKEN) {
        if (timer_elapsed32(sleep_timer) > 2*60*1000) { // 2 minutes 
             jiggler_token = defer_exec(1, jiggler_callback, NULL);
             init_matrixAni();
        }
    }
}
  




#ifdef ENCODER_MAP_ENABLE
    const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {0};
#endif

#if defined(ENCODER_MAP_ENABLE)
    const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
        [_QT] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_FN] =  { ENCODER_CCW_CW(KC_RIGHT, KC_LEFT),      ENCODER_CCW_CW(KC_PGUP, KC_PGDN)  },
        [_MO] =  { ENCODER_CCW_CW(MS_WHLU , MS_WHLD),      ENCODER_CCW_CW(MS_WHLR, MS_WHLL)  },
        [_NM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_BRID, KC_BRIU)  },
        [_CM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_CN] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
        [_GM] =  { ENCODER_CCW_CW(KC_UP   , KC_DOWN),      ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  }
    };
#endif


#ifdef OLED_ENABLE

// base icons
static const char        top[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0};
static const char       eyes[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0};
static const char      mouth[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};

static const char      GLtop[] = {0x94, 0x95, 0x96, 0x97, 0x98, 0};
static const char     GLeyes[] = {0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0};
static const char    GLmouth[] = {0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0};

// icon options
 static const char mindblown[] = {0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0}; // top
 static const char       cat[] = {0x80, 0x86, 0x87, 0x88, 0x84, 0}; // top
 static const char      pong[] = {0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0}; // eyes
 static const char   bigEyes[] = {0x8A, 0x20, 0x8C, 0x20, 0x8E, 0}; // eyes
 static const char mouthOpen[] = {0xc0, 0xb0, 0xb1, 0xb2, 0xc4, 0}; // mouth
 static const char   vampire[] = {0xc0, 0xd0, 0xd1, 0xd2, 0xc4, 0}; // mouth


 // Fade effect function
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


 void drawscull(void) {
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
    if (is_caps_word_on()) {
        oled_set_cursor(8,2);
        oled_write(bigEyes, false);
    }
    if (jiggler_enable) {
        oled_set_cursor(0,3);
        oled_write_char(0xCA, false);
    } else {
        oled_set_cursor(0,3);
        oled_write_char(' ', false);
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
    static  uint32_t saveTime = 3*60*1000; // 3 minutes
    static  uint32_t sleepTime = 15*60*1000; // 15 minutes

    if (jiggler_token != INVALID_DEFERRED_TOKEN || timer_elapsed32(sleep_timer) < saveTime) {
        // Run animation if Jiggler is ON or if keyboard is Active
        static uint32_t matrix_timer = 0;
        if (timer_elapsed32(matrix_timer) > 120) { // Rain speed
            matrix_timer = timer_read32();
            update_matrix();
            if (timer_elapsed32(sleep_timer) < saveTime) {
                // Regular mode: Draw Skull (active face)
                 oled_on();
                 drawscull();
            } else {
                // Jiggler mode with timeout exceeded? shouldn't hit here due to logic overlap
                 drawscull();
            }
            
            draw_matrix();
        }
    }
   
    if (jiggler_token != INVALID_DEFERRED_TOKEN) {
         return false; // Jiggler handles its own flow (and the block above handles visuals)
    }

    if(timer_elapsed32(sleep_timer) < saveTime) {
        // oled_on(); // Handled in animation loop
        // drawscull(); // Handled in animation loop
        } else if (timer_elapsed32(sleep_timer) < sleepTime) {
           screen_save();
      } else {
         fade_display();
      }
       return false;
     }


#endif  //end oled


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
