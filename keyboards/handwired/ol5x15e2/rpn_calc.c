// Ref  https://github.com/apoluekt/OpenRPNCalc)


#include "rpn_calc.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14159265358979323846

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
#define OP_ABS 0x101F

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

double trigconv = 0.017453292519943295; // Default PI/180
static void set_trigconv(void) {
    trigconv = (trigmode == 0) ? M_PI / 180.0 : 1.0;
}

int context = CONTEXT_REAL;
int precision = 4;
int precision_uncert = 4;


// --------------------------------------------------------------------------
// Helper Functions (Stack & Logic)
// --------------------------------------------------------------------------

// CL_CLRS
void clear_stack(void) {
    memset(stack, 0, sizeof(stack));
    memset(stack2, 0, sizeof(stack2));
    lastx = 0.0;
    lastx2 = 0.0;
    error_flag = 0;
    stack_size = 0;
}

void clear_input(void) {
    memset(&input, 0, sizeof(input));
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
        stack2[stack_size] = 0;
	}
}


void stack_rotate_up(void) {
    if (stack_size < 2) return;
    double tmp = stack[stack_size-1];
    double tmp2 = stack2[stack_size-1];
    for (int i=stack_size-1; i>0; i--) {
        stack[i] = stack[i-1];
        stack2[i] = stack2[i-1];
    }
    stack[0] = tmp;
    stack2[0] = tmp2;
}

void stack_rotate_down(void) {
    if (stack_size < 2) return;
    double tmp = stack[0];
    double tmp2 = stack2[0];
    for (int i=0; i<stack_size-1; i++) {
        stack[i] = stack[i+1];
        stack2[i] = stack2[i+1];
    }
    stack[stack_size-1] = tmp;
    stack2[stack_size-1] = tmp2;
}


// --------------------------------------------------------------------------
// Input Parsing & Conversion
// --------------------------------------------------------------------------

// Helper to avoid pow() for integer parsing
double my_pow10(int n) {
    double r = 1.0;
    if (n >= 0) {
        for(int i=0; i<n; i++) r *= 10.0;
    } else {
        for(int i=0; i<-n; i++) r /= 10.0;
    }
    return r;
}

double convert_input(void) {
	int i;
	double number = 0.;
	double shift = 1;

	for (i=0; i<input.mpos; i++) {
		number += (input.mantissa[input.mpos-i-1]) * shift;
		shift *= 10;
	}
	int exponent = 100*input.exponent[2] + 10*input.exponent[1] + input.exponent[0];
	if (input.expsign) exponent = -exponent;
	if (input.point) exponent -= (input.mpos-input.point);

    if (exponent != 0) {
	    number *= my_pow10(exponent);
    }
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
    if (stack_size == 0) {
        stack_size = 1;
    }
	input.replace_x = 1;
	stack_push(stack[0], stack2[0]);
}

void apply_op(uint16_t code) {
    maybe_convert_input();
    input.replace_x = 0;

    // Safety check for trigconv
    if (trigconv == 0.0) set_trigconv();

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
        case OP_DIV:
            if (x == 0.0) {
                error_flag = 1;
                stack[0] = 0.0;
                return;
            }
            res = y / x; binary = true; break;
        case OP_POW: res = pow(y, x); binary = true; break;

        // Unary
        case OP_SQRT: res = sqrt(x); break;
        case OP_SQR: res = x*x; break;
        case OP_SIN: res = sin(trigconv*x); break;
        case OP_COS: res = cos(trigconv*x); break;
        case OP_TAN: res = tan(trigconv*x); break;
        case OP_LN: res = log(x); break;
        case OP_LG: res = log10(x); break;
        case OP_INV:
            if (x == 0.0) {
                error_flag = 1;
                stack[0] = 0.0;
                return;
            }
            res = 1.0 / x; break;
        case OP_ASIN: res = asin(x); if (!trigmode) res /= trigconv; break;
        case OP_ACOS: res = acos(x); if (!trigmode) res /= trigconv; break;
        case OP_ATAN: res = atan(x); if (!trigmode) res /= trigconv; break;
        case OP_ABS: res = fabs(x); break;
        case OP_CONST_PI:
            stack_push(M_PI, 0);
            return;

        // Stack
        case OP_DROP: stack_drop(); return;
        case OP_SWAP:
            if (stack_size >= 2) {
                double tmp = stack[0];
                stack[0] = stack[1];
                stack[1] = tmp;
            }
            return;
        case OP_CLEAR_STACK:
            clear_stack();
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
        case CL_BKS:
            enter_backspace(); return true;
        case CL_ENT:
            enter_enter(); return true;

        case CL_PLUS:
        case KC_PLUS:
        case KC_PPLS: apply_op(OP_PLUS); return true;
        case CL_MINS:
        case KC_MINS:
        case KC_PMNS: apply_op(OP_MINUS); return true;
        case CL_MULT:
        case KC_ASTR:
        case KC_PAST: apply_op(OP_MULT); return true;
        case CL_DIV:
        case KC_SLSH:
        case KC_PSLS: apply_op(OP_DIV); return true;
        case CL_POW:
        case CL_XtY:
        case KC_CIRC: apply_op(OP_POW); return true;

        // Extended Functions
        case CL_CLRS: apply_op(OP_CLEAR_STACK); return true;
        case CL_XxY: apply_op(OP_SWAP); return true;
        case CL_CLRX:
        case CL_CE:
            if (input.started) {
                 clear_input();
                 stack[0] = 0;
            } else {
                 stack[0] = 0;
            }
            return true;

        case CL_INV: apply_op(OP_INV); return true;
        case CL_SQ:  apply_op(OP_SQR); return true;
        case CL_SQRT: apply_op(OP_SQRT); return true;
        case CL_ABS: apply_op(OP_ABS); return true;

        case CL_SIN: apply_op(OP_SIN); return true;
        case CL_COS: apply_op(OP_COS); return true;
        case CL_TAN: apply_op(OP_TAN); return true;
        case CL_ASIN: apply_op(OP_ASIN); return true;
        case CL_ACOS: apply_op(OP_ACOS); return true;
        case CL_ATAN: apply_op(OP_ATAN); return true;

        case CL_LN: apply_op(OP_LN); return true;
        case CL_LOG: apply_op(OP_LG); return true;
        case CL_PI: apply_op(OP_CONST_PI); return true;

        default: return false;
    }
}

void calc_push(double val) {
    maybe_convert_input();
    stack_push(val, 0);
}

static void format_numeric_value(double num, char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return;

    if (!isfinite(num)) {
        snprintf(buffer, buffer_size, "err");
        return;
    }

    if (num == 0.0) {
        snprintf(buffer, buffer_size, "0");
        return;
    }

    char formatted[32];
    size_t pos = 0;
    if (num < 0.0) {
        formatted[pos++] = '-';
        num = -num;
    }

    int exponent = 0;
    while (num >= 10.0) {
        num /= 10.0;
        exponent++;
    }
    while (num < 1.0) {
        num *= 10.0;
        exponent--;
    }

    unsigned int mantissa = (unsigned int)(num * 100000.0 + 0.5);
    if (mantissa >= 1000000U) {
        mantissa /= 10U;
        exponent++;
    }

    char digits[7];
    snprintf(digits, sizeof(digits), "%06u", mantissa);

    if (exponent >= 6 || exponent < -4) {
        formatted[pos++] = digits[0];
        size_t last_digit = 6;
        while (last_digit > 1 && digits[last_digit - 1] == '0') {
            last_digit--;
        }
        if (last_digit > 1) {
            formatted[pos++] = '.';
            memcpy(&formatted[pos], &digits[1], last_digit - 1);
            pos += last_digit - 1;
        }
        int exponent_length = snprintf(&formatted[pos], sizeof(formatted) - pos, "e%+d", exponent);
        if (exponent_length < 0 || (size_t)exponent_length >= sizeof(formatted) - pos) {
            snprintf(buffer, buffer_size, "err");
            return;
        }
        pos += (size_t)exponent_length;
    } else if (exponent < 0) {
        formatted[pos++] = '0';
        formatted[pos++] = '.';
        for (int i = 0; i < -exponent - 1; i++) {
            formatted[pos++] = '0';
        }
        memcpy(&formatted[pos], digits, sizeof(digits) - 1);
        pos += sizeof(digits) - 1;
        while (formatted[pos - 1] == '0') {
            pos--;
        }
    } else {
        int integer_digits = exponent + 1;
        int digits_to_copy = integer_digits < 6 ? integer_digits : 6;
        memcpy(&formatted[pos], digits, digits_to_copy);
        pos += (size_t)digits_to_copy;
        while (integer_digits > 6) {
            formatted[pos++] = '0';
            integer_digits--;
        }

        if (digits_to_copy < 6) {
            formatted[pos++] = '.';
            memcpy(&formatted[pos], &digits[digits_to_copy], 6 - digits_to_copy);
            pos += (size_t)(6 - digits_to_copy);
            while (formatted[pos - 1] == '0') {
                pos--;
            }
            if (formatted[pos - 1] == '.') {
                pos--;
            }
        }
    }

    formatted[pos] = '\0';
    snprintf(buffer, buffer_size, "%s", formatted);
}

void calc_output_result(void) {
    maybe_convert_input();
    if (stack_size > 0) {
        char buf[32];
        format_numeric_value(stack[0], buf, sizeof(buf));
        send_string(buf);
    }
}


// --------------------------------------------------------------------------
// Display Rendering (String Based)
// --------------------------------------------------------------------------

void format_number(double num, char *buffer) {
    format_numeric_value(num, buffer, 22);
}

void calc_get_line(uint8_t line, char *buffer) {
    // Line 0: Input or X
    // Line 1: Y
    // Line 2: Z
    // Line 3: T

    char temp[22];
    if (line == 0) {
        if (input.started) {
            // Render Input Buffer
            int p = 0;
            if (input.sign) temp[p++] = '-';
            for (int i = 0; i < input.mpos; i++) {
                temp[p++] = input.mantissa[i] + '0';
                if (input.point > 0 && (i + 1) == input.point) temp[p++] = '.';
            }

            if (input.exponent[0] || input.exponent[1] || input.exponent[2] || input.expentry) {
                temp[p++] = 'E';
                if (input.expsign) temp[p++] = '-';
                else temp[p++] = '+';

                int exponent_digits = 0;
                for (int i = 0; i < 3; i++) {
                    char digit = input.exponent[i];
                    if (digit != 0 || exponent_digits > 0 || i == 2) {
                        temp[p++] = digit + '0';
                        exponent_digits++;
                    }
                }
            }

            if (input.point == 0 && input.mpos == 0) {
                temp[p++] = '_';
            }
            temp[p] = '\0';
        } else {
            // Render X
            if (stack_size > 0) {
                char nb[22];
                format_number(stack[0], nb);
                snprintf(temp, sizeof(temp), "x: %s", nb);
            } else {
                strcpy(temp, "x: 0");
            }
        }
    } else {
        // Stack Lines: 1->Y, 2->Z, 3->T. These are fixed RPN registers;
        // display their contents independently of the current stack depth.
        int idx = line; // 1->Y, 2->Z, 3->T
        double val = (idx < MAX_STACK_SIZE) ? stack[idx] : 0.0;

        char nb[22];
        format_number(val, nb);
        char reg = (idx == 1) ? 'y' : (idx == 2) ? 'z' : (idx == 3) ? 't' : ('0' + idx);
        snprintf(temp, sizeof(temp), "%c: %s", reg, nb);
    }

    // Pad with spaces to clear line (OLED width ~21 chars)
    int len = strlen(temp);
    memset(buffer, ' ', 21);
    buffer[21] = '\0';
    if (len > 21) len = 21;
    memcpy(buffer, temp, len);
}
