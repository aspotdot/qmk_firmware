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
