#include "rpn_calc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// --------------------------------------------------------------------------
// Constants & Defines (Code Imported from OpenRPNCalc)
// --------------------------------------------------------------------------

#define MAX_STACK_SIZE 12
#define MAX_MEMORY_SIZE 12


// Context constants
#define CONTEXT_REAL 0
#define CONTEXT_UNCERT 1
#define CONTEXT_COMPLEX 2
#define CONTEXT_INTEGER 3

// Internal Opcodes
#define OP_ENTER_0 0x5000
#define OP_ENTER_1 0x5001
#define OP_ENTER_2 0x5002
#define OP_ENTER_3 0x5003
#define OP_ENTER_4 0x5004
#define OP_ENTER_5 0x5005
#define OP_ENTER_6 0x5006
#define OP_ENTER_7 0x5007
#define OP_ENTER_8 0x5008
#define OP_ENTER_9 0x5009
#define OP_ENTER_DECPOINT 0x500A
#define OP_ENTER_SIGN 0x500B
#define OP_ENTER_EXP 0x500C
#define OP_ENTER_BACKSPACE 0x500D
#define OP_ENTER_UNCERT 0x500E
#define OP_ENTER 0x500F

#define OP_DROP 0x6001
#define OP_SWAP 0x6002
#define OP_LASTX 0x6003
#define OP_CLEAR_STACK 0x6004
#define OP_ROTUP 0x6007
#define OP_ROTDOWN 0x6008
#define OP_CLEAR_MEM 0x6009

#define OP_PLUS 0x2001
#define OP_MINUS 0x2002
#define OP_MULT 0x2003
#define OP_DIV 0x2004
#define OP_POW 0x2008
#define OP_ROOTX 0x2013
#define OP_CYX 0x2014
#define OP_PYX 0x2015
#define OP_SIGNIF_XY 0x2016
#define OP_POISSON 0x2017
#define OP_CHI2_PROB 0x2018

#define OP_INV 0x1005
#define OP_SQR 0x1006
#define OP_SQRT 0x1007
#define OP_LN 0x1009
#define OP_LG 0x100A
#define OP_EXP 0x100B
#define OP_POW10 0x100C
#define OP_SIN 0x100D
#define OP_COS 0x100E
#define OP_TAN 0x100F
#define OP_ASIN 0x1010
#define OP_ACOS 0x1011
#define OP_ATAN 0x1012
#define OP_ERF 0x1014
#define OP_ERFINV 0x1015
#define OP_GAMMA 0x1016
#define OP_LOGGAMMA 0x1017
#define OP_GAMMABETA 0x1018
#define OP_BETAGAMMA 0x1019
#define OP_ETATHETA 0x101A
#define OP_THETAETA 0x101B
#define OP_SIGNIF_X 0x101C
#define OP_GAUSS_PVALUE 0x101D
#define OP_FACTORIAL 0x101E

#define OP_POLAR 0x3001
#define OP_DESCARTES 0x3002

#define OP_CONST_PI 0x4013
#define OP_CONST_E 0x4014

// --------------------------------------------------------------------------
// State Variables
// --------------------------------------------------------------------------

int stack_size = 0;
double stack[MAX_STACK_SIZE]; // Stack values {X, Y, Z, T}
double lastx;               // Last X value
int error_flag = 0;         // Error flag

double stack2[MAX_STACK_SIZE]; // Stack for ERRORs in UNCERT context
double lastx2;              // Last X ERROR in UNCERT context

double variables[MAX_MEMORY_SIZE];      // Storage space for variables
double variables2[MAX_MEMORY_SIZE];     // Storage space for variable errors

typedef struct {
	char mantissa[12];   // Mantissa digits array
	char sign;           // Sign: 0 for "+", 1 for "-"
	char exponent[3];    // Exponent digits array
	char expsign;        // Sign of the exponent: 0 for "+", 1 for "-"
	char started;        // 1 if input mode is active
	char replace_x;      // 1 if "ENTER" or "C" key was just pressed
	char expentry;       // 1 if exponent is being entered
	int8_t point;        // Decimal point position
	int8_t mpos;         // Number of mantissa digits entered
} t_input;

t_input input;

// Config state
int trigmode = 0;     // 0-DEG, 1-RAD

double trigconv = M_PI/180.0;
int context = CONTEXT_REAL;
int precision = 4;         
int precision_uncert = 4;




// --------------------------------------------------------------------------
// Helper Functions (Stack & Logic)
// --------------------------------------------------------------------------

void clear_stack(void) {
	stack[0] = 0.; stack2[0] = 0.;
	lastx = 0.; lastx2 = 0.;
	error_flag = 0; stack_size = 0;
    // Ensure stack is clean for safety
    for(int i=0; i<MAX_STACK_SIZE; i++) stack[i] = 0.0;
}

void clear_input(void) {
	input.mpos = 0; input.sign = 0;
	input.point = 0; input.started = 0;
	input.replace_x = 0; input.expentry = 0;
	memset(input.exponent, 0, 3);
	input.expsign = 0;
}

void clear_variables(void) {
	for(int i=0; i<MAX_MEMORY_SIZE; i++) {
		variables[i] = 0.0;
		variables2[i] = 0.0;
	}
}

void stack_push(double num, double err) {
	if (stack_size < MAX_STACK_SIZE) stack_size++;
	for (int i=stack_size-1; i>0; i--) {
		stack[i] = stack[i-1];
		stack2[i] = stack2[i-1];
	}
	stack[0] = num;
	stack2[0] = err;
}

void stack_drop(void) {
	if (stack_size>0) {
		stack_size--;
		for (int i=0; i<stack_size; i++) {
			stack[i] = stack[i+1];
			stack2[i] = stack2[i+1];
		}
        stack[stack_size] = 0; // Clear visible residue
	}
}

void stack_rotate_up(void) {
    if (stack_size < 2) return;
	double tmp = stack[stack_size-1];
	// double tmp2 = stack2[stack_size-1];
	for (int i=stack_size-1; i>0; i--) {
		stack[i] = stack[i-1];
		// stack2[i] = stack2[i-1];
	}
	stack[0] = tmp;
	// stack2[0] = tmp2;
}

void set_trigconv(void) {
	if (trigmode == 0) trigconv = M_PI/180.;
	else trigconv = 1.0;
}

// --------------------------------------------------------------------------
// Input Parsing & Conversion
// --------------------------------------------------------------------------

double convert_input(void) {
	int i;
	double number = 0.;
	double shift = 1;

	for (i=0; i<input.mpos; i++) {
		number += input.mantissa[input.mpos-i-1]*shift;
		shift *= 10;
	}
	int exponent = 100*input.exponent[2] + 10*input.exponent[1] + input.exponent[0];
	if (input.expsign) exponent = -exponent;
	if (input.point) exponent -= (input.mpos-input.point);

	number *= pow(10, exponent);
	if (input.sign) number = -number;

	if (!isfinite(number)) {
		error_flag = 1;
	}
	return number;
}

void maybe_convert_input(void) {
	if (input.started) {
        // Only handling real context for now
		stack[0] = convert_input();
		clear_input();
	}
}

// --------------------------------------------------------------------------
// Logical Operations (Simplified)
// --------------------------------------------------------------------------

void enter_number(char c) {
	if (!input.started) {
		if (!input.replace_x && !error_flag) {
			stack_push(0, 0);
		}
		error_flag = 0;
		input.started = 1;
		input.replace_x = 0;
        stack[0] = 0; // Visual placeholder
	}
	if (input.expentry == 0) {
		if (input.mpos < 10) { 
			input.mantissa[input.mpos++] = c;
		}
	} else {
		input.exponent[2] = input.exponent[1];
		input.exponent[1] = input.exponent[0];
		input.exponent[0] = c;
	}
}

void enter_decpoint(void) {
	if (!input.started) {
		if (!input.replace_x && !error_flag) stack_push(0, 0);
		error_flag = 0;
		input.started = 1;
		input.replace_x = 0;
	}
	if (input.expentry == 0 && input.point == 0) {
		if (input.mpos == 0) input.mantissa[input.mpos++] = 0;
		input.point = input.mpos;
	}
}

void enter_backspace(void) {
    if (input.started) {
        if (input.mpos > 0) {
             input.mpos--;
             if (input.mpos == 0) input.started = 0; 
             // Logic simplified: just backspace last char
        } else {
            input.started = 0;
        }
    } else {
        stack_drop();
    }
}

void enter_enter(void) {
	maybe_convert_input();
	if (error_flag) return;
	input.replace_x = 1;
	stack_push(stack[0], stack2[0]);
}

void apply_op(uint16_t code) {
    maybe_convert_input();
    input.replace_x = 0;
    
    if (stack_size < 1) return;
    double x = stack[0];
    double y = (stack_size > 1) ? stack[1] : 0;
    double res = x;
    bool binary = false;
    
    switch(code) {
        // Binary
        case OP_PLUS: res = y + x; binary = true; break;
        case OP_MINUS: res = y - x; binary = true; break;
        case OP_MULT: res = y * x; binary = true; break;
        case OP_DIV: res = (x!=0) ? y / x : 0; binary = true; break; // Div0 check needed 
        case OP_POW: res = pow(y, x); binary = true; break;
        
        // Unary
        case OP_SQRT: res = sqrt(x); break;
        case OP_SQR: res = x*x; break;
        case OP_SIN: res = sin(trigconv*x); break;
        case OP_COS: res = cos(trigconv*x); break;
        case OP_TAN: res = tan(trigconv*x); break;
        case OP_LN: res = log(x); break;
        case OP_LG: res = log10(x); break;
        case OP_INV: res = (x!=0) ? 1.0/x : 0.0; break;
        
        // Stack
        case OP_DROP: stack_drop(); return; 
        case OP_SWAP: 
            if (stack_size >= 2) {
                stack[0] = y; stack[1] = x;
            }
            return;
    }
    
    if (binary) stack_drop();
    stack[0] = res;
    if (!isfinite(stack[0])) error_flag = 1;
}

// --------------------------------------------------------------------------
// Public Interfaces
// --------------------------------------------------------------------------

void calc_init(void) {
    clear_stack();
    clear_input();
    clear_variables();
    set_trigconv();
}

enum custom_keycodes {
  CL_OUT ,
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
  CL_SQ 
};





bool calc_handle_key(uint16_t keycode) {
    // Map QMK Keycodes to Internal Functions
    
    // Digits
    if (keycode >= KC_1 && keycode <= KC_0) {
        enter_number((keycode == KC_0) ? 0 : (keycode - KC_1 + 1));
        return true;
    }
    if (keycode >= KC_P1 && keycode <= KC_P0) {
        enter_number((keycode == KC_P0) ? 0 : (keycode - KC_P1 + 1));
        return true;
    }
    
    switch (keycode) {
        case KC_DOT: 
        case KC_PDOT:
            enter_decpoint(); return true;
        case KC_BSPC: 
            enter_backspace(); return true;
        case CL_ENT: 
            enter_enter(); return true;
            
        case KC_PLUS: 
        case KC_PPLS: apply_op(OP_PLUS); return true;
        case KC_MINS: 
        case KC_PMNS: apply_op(OP_MINUS); return true;
        case KC_ASTR: 
        case KC_PAST: apply_op(OP_MULT); return true;
        case KC_SLSH: 
        case KC_PSLS: apply_op(OP_DIV); return true;
        case KC_CIRC: apply_op(OP_POW); return true;
        
        default: return false; 
    }
}

void calc_push(double val) {
    maybe_convert_input();
    stack_push(val, 0);
}

void calc_output_result(void) {
    maybe_convert_input();
    if (stack_size > 0) {
        char buf[32];
        // Simple float output
        int i_part = (int)stack[0];
        int f_part = (int)(fabs(stack[0] - i_part) * 10000);
        snprintf(buf, sizeof(buf), "%s%d.%04d", (stack[0]<0 && i_part==0)?"-":"", i_part, f_part);
        send_string(buf);
    }
}


// --------------------------------------------------------------------------
// Display Rendering (String Based)
// --------------------------------------------------------------------------

void format_number(double num, char *buffer) {
    // Basic formatting for now. 
    // We will stick to a reasonable default: %.5g
    snprintf(buffer, 21, "%.5g", num); 
}

void calc_get_line(uint8_t line, char *buffer) {
    // Line 0: Input or X
    // Line 1: Y
    // Line 2: Z
    // Line 3: T
    
    if (line == 0) {
        if (input.started) {
            // Render Input Buffer
            int p = 0;
            if(input.sign) buffer[p++] = '-';
            for (int i=0; i<input.mpos; i++) {
                buffer[p++] = input.mantissa[i] + '0';
                if (input.point > 0 && (i+1) == input.point) buffer[p++] = '.';
            }
            if (input.point == 0) buffer[p++] = '_'; // Cursor
            buffer[p] = '\0';
        } else {
            // Render X
            if (stack_size > 0) {
                char nb[20];
                format_number(stack[0], nb);
                snprintf(buffer, 22, "X: %s", nb);
            } else {
                strcpy(buffer, "X: 0");
            }
        }
    } else {
        // Stack Lines
        int idx = line; // 1->Y(stack[1]), 2->Z(stack[2]), 3->T(stack[3])
        if (idx < stack_size) {
            char nb[20];
            format_number(stack[idx], nb);
            char reg = (idx==1)?'Y':(idx==2)?'Z':(idx==3)?'T':('0'+idx);
            snprintf(buffer, 22, "%c: %s", reg, nb);
        } else {
            buffer[0] = '\0'; // Empty line
        }
    }
}
