#include "oled.h"
#include "rpn_calc.h"

#ifdef OLED_ENABLE

static uint32_t sleep_timer = 0;
static uint32_t glitch_timer = 0;
static uint32_t matrix_timer = 0;

void reset_oled_timer(void) {
    sleep_timer = timer_read32();
}

// Matrix Rain Implementation
#define DRAINS_COLS 21 // 128 pixels / 6px font width = 21.3
#define DRAINS_ROWS 4  // 32 pixels / 8px font height
// 127 will represent "inactive" or "off screen" safely above DRAINS_ROWS
static int8_t matrix_drops[DRAINS_COLS];
static uint8_t matrix_speeds[DRAINS_COLS];
static char matrix_col_chars[DRAINS_COLS];

// From ref_files/myskeeb/oled.c
static const char PROGMEM code_to_name[0xFF] = {
//   0    1    2    3    4    5    6    7    8    9    A    B    c    D    E    F
    182, ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',  // 0x
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',  // 1x
    '3', '4', '5', '6', '7', '8', '9', '0',  20,  19,  17,  29,  22, '-', '=', '[',  // 2x
    ']','\\', '#', ';','\'', '`', ',', '.', '/', 188, 149, 150, 151, 152, 153, 154,  // 3x
    155, 156, 157, 158, 159, 181, 191, 190, ' ', ' ', 185, 183,  16, 186, 184,  26,  // 4x
     27,  25,  24, 189, '/', '*', '-', '+', ' ', '1', '2', '3', '4', '5', '6', '7',  // 5x
    '8', '9', '0', '.', ' ', 187, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 6x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 7x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 8x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 9x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 214, 215, 216, 217, 218, 219, 220, 221,  // Ax
    ' ', ' ', 213, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Bx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Cx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Dx
    'C', 'S', 'A', 'W', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ex
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '        // Fx
};

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
        if (matrix_drops[i] > DRAINS_ROWS + 4) {
         if (is_jiggler_active()) {
             // Loop if Jiggler is ON
             matrix_drops[i] = -(rand() % 10);
             matrix_speeds[i] = (rand() % 4) + 1;
             matrix_col_chars[i] = 0x01 + (rand() % (0xDF - 0x01));
         } else {
             // Stop if Jiggler is OFF
             matrix_drops[i] = 127; 
            }
        }
    
        // If Jiggler is ON and drop is inactive, chance to start it
        if (is_jiggler_active() && matrix_drops[i] == 127) {
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

void trigger_matrix_rain(uint16_t keycode) {
    char charDrop = 0;
    
    // Use code_to_name table safely
    if (keycode < 0xFF) {
        charDrop = pgm_read_byte(&code_to_name[keycode]);
    } else {
        // Fallback for keycodes >= 0xFF if necessary, or just ignore
        // Or specific mapping for some logical keys
    }

    if (charDrop && charDrop != ' ') {
        int col = rand() % DRAINS_COLS;
        matrix_drops[col] = 0; // Start at top
        matrix_col_chars[col] = charDrop;
        matrix_speeds[col] = 2; // Fast drop for typing 
    }
}

// Icons
static const char        top[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0};
static const char       eyes[] = {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0};
static const char      mouth[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};

static const char      GLtop[] = {0x94, 0x95, 0x96, 0x97, 0x98, 0};
static const char     GLeyes[] = {0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0};
static const char    GLmouth[] = {0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0};

// icon options
static const char mindblown[] = {0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0}; // top
//static const char       cat[] = {0x80, 0x86, 0x87, 0x88, 0x84, 0}; // top
static const char      pong[] = {0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0}; // eyes
static const char   bigEyes[] = {0x8A, 0x20, 0x8C, 0x20, 0x8E, 0}; // eyes
static const char mouthOpen[] = {0xc0, 0xb0, 0xb1, 0xb2, 0xc4, 0}; // mouth
static const char   vampire[] = {0xc0, 0xd0, 0xd1, 0xd2, 0xc4, 0}; // mouth

// Forward declare custom layers
enum layer_names_oled_ref {
    _QT,
    _FN,
    _MO,
    _NM,
    _CM,
    _CN,
    _GM,
    _CL
};

static void drawscull(void) {
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
    if (is_jiggler_enabled()) {
        oled_set_cursor(0,3);
        oled_write_char(0xCA, false);
    } else {
        oled_set_cursor(0,3);
        oled_write_char(' ', false);
    }
}

 // Fade effect function
const uint8_t single_bit_masks[8] = {127, 191, 223, 239, 247, 251, 253, 254}; 
static void fade_display(void) {
    //Define the reader structure
    oled_buffer_reader_t reader;
    uint8_t buff_char;
    if (random() % 30 == 0) {
        srand(timer_read());
        reader = oled_read_raw(0);
        for (uint16_t i = 0; i < reader.remaining_element_count; i++) {
            buff_char = *reader.current_element;
            if (buff_char != 0) {
                oled_write_raw_byte(buff_char & single_bit_masks[rand() % 8], i);
            }
            reader.current_element++;
        }
    }
}

  void screen_save(void) {
       if (timer_elapsed32(glitch_timer) > 100) { 
         glitch_timer = timer_read32();
         uint8_t GLpick = rand()%(6-1+1)+1;  
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

void keyboard_post_init_user(void) {
    init_matrixAni();
    calc_init();
}

bool oled_task_user(void) {
    static  uint32_t saveTime = 3*60*1000; // 3 minutes
    static  uint32_t sleepTime = 15*60*1000; // 15 minutes

     // Calculator Display Priority
     if (get_highest_layer(layer_state) == _CL) {
         oled_on();
         char line_buf[22];
         
         // Top line (Stack 2)
         oled_set_cursor(0, 0);
         calc_get_line(3, line_buf);
         oled_write(line_buf, false);
         
         // Stack 1
         oled_set_cursor(0, 1);
         calc_get_line(2, line_buf);
         oled_write(line_buf, false);
         
         // Stack 0
         oled_set_cursor(0, 2);
         calc_get_line(1, line_buf);
         oled_write(line_buf, false);
         
         // Input / Result
         oled_set_cursor(0, 3);
         calc_get_line(0, line_buf);
         oled_write(line_buf, false);
         
         // Reset sleep timer while using calc
         reset_oled_timer();

         return false;
    }

    if (is_jiggler_enabled() || timer_elapsed32(sleep_timer) < saveTime) {
        // Run animation if Jiggler is ON or if keyboard is Active
        if (timer_elapsed32(matrix_timer) > 120) { // Rain speed
            matrix_timer = timer_read32();
            update_matrix();
            if (timer_elapsed32(sleep_timer) < saveTime) {
                // Regular mode: Draw Skull (active face)
                 oled_on();
                 drawscull();
            } else {
                 drawscull();
            }
            
            draw_matrix();
        }
    }
   
    if (is_jiggler_active()) {
         return false; 
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

#endif
