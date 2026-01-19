#pragma once

#include "quantum.h"

// Initialize the calculator state
void calc_init(void);

// Process a key event when in calculator mode
// Returns true if the key was handled, false otherwise
bool calc_handle_key(uint16_t keycode);

// Get a formatted string for a specific line of the display
// line: 0 = Input/Bottom, 1 = Stack[0], 2 = Stack[1], etc.
// buffer: char array to write to (ensure enough size, e.g., 22 chars for OLED)
void calc_get_line(uint8_t line, char *buffer);

// Helper to push a value (exposed for macros if needed)
void calc_push(double val);

// Output the top of the stack to the host via SEND_STRING
void calc_output_result(void);

enum calc_keycodes {
  CL_KEY_START = SAFE_RANGE + 0x40, // Offset to avoid collision with keymap
  CL_OUT,
  CL_CLRS, 
  CL_INV , 
  CL_SIN , 
  CL_ASIN, 
  CL_LN  , 
  CL_CLRX, 
  CL_PI  , 
  CL_COS , 
  CL_ACOS, 
  CL_LOG , 
  CL_XxY , 
  CL_SQRT, 
  CL_TAN , 
  CL_ATAN, 
  CL_ABS ,
  CL_ENT ,
  CL_SQ,
  CL_CE,
  CL_XtY,
  CL_PLUS,
  CL_MINS,
  CL_MULT,
  CL_DIV,
  CL_POW,
  CL_BKS
};

#define CL_1 KC_1
#define CL_2 KC_2
#define CL_3 KC_3
#define CL_4 KC_4
#define CL_5 KC_5
#define CL_6 KC_6
#define CL_7 KC_7
#define CL_8 KC_8
#define CL_9 KC_9
#define CL_0 KC_0
#define CL_DOT KC_DOT
