#pragma once

#include "quantum.h"

// Defined in keymap.c
extern bool is_jiggler_active(void);
extern bool is_jiggler_enabled(void);

// OLED Public API
void reset_oled_timer(void);
void trigger_matrix_rain(uint16_t keycode);

// Custom OLED task (if not using weak reference override, but here we override oled_task_user)
// bool process_oled_user_custom(void); 
