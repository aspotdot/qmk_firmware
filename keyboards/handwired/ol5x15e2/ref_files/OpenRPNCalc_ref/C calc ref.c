/*

acos(x) 	Returns the arccosine of x, in radians
acosh(x) 	Returns the hyperbolic arccosine of x
asin(x) 	Returns the arcsine of x, in radians
asinh(x) 	Returns the hyperbolic arcsine of x
atan(x) 	Returns the arctangent of x as a numeric value between -PI/2 and PI/2 radians
atan2(y, x) Returns the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta)
atanh(x) 	Returns the hyperbolic arctangent of x
cbrt(x) 	Returns the cube root of x
cos(x) 	    Returns the cosine of x (x is in radians)
cosh(x) 	Returns the hyperbolic cosine of x
exp(x) 	    Returns the value of Ex
expm1(x) 	Returns ex-1
log(x) 	    Returns the natural logarithm of x
log10(x) 	Returns the base 10 logarithm of x
log1p(x) 	Returns the natural logarithm of x+1
log2(x) 	Returns the base 2 logarithm of the absolute value of x
logb(x) 	Returns the floating-point base logarithm of the absolute value of x
pow(x, y) 	Returns the value of x to the power of y
sin(x) 	    Returns the sine of x (x is in radians)
sinh(x) 	Returns the hyperbolic sine of x
sqrt(x) 	Returns the square root of x
tan(x) 	    Returns the tangent of x (x is in radians)
tanh(x) 	Returns the hyperbolic tangent of x





ceil(x) 	Returns the value of x rounded up to its nearest integer
copysign(x, y) 	Returns the first floating point x with the sign of the second floating point y
exp2(x) 	Returns the value of 2x
erf(x) 	Returns the value of the error function at x
erfc(x) 	Returns the value of the complementary error function at x
fabs(x) 	Returns the absolute value of x
fdim(x) 	Returns the positive difference between x and y
floor(x) 	Returns the value of x rounded down to its nearest integer
fma(x, y, z) 	Returns x*y+z without losing precision
fmax(x, y) 	Returns the highest value of a floating x and y
fmin(x, y) 	Returns the lowest value of a floating x and y
fmod(x, y) 	Returns the floating point remainder of x/y
frexp(x, y) 	With x expressed as m*2n, returns the value of m (a value between 0.5 and 1.0) and writes the value of n to the memory at the pointer y
hypot(x, y) 	Returns sqrt(x2 +y2) without intermediate overflow or underflow
ilogb(x) 	Returns the integer part of the floating-point base logarithm of x
ldexp(x, y) 	Returns x*2y
lgamma(x) 	Returns the logarithm of the absolute value of the gamma function at x
llrint(x) 	Rounds x to a nearby integer and returns the result as a long long integer
llround(x) 	Rounds x to the nearest integer and returns the result as a long long integer
lrint(x) 	Rounds x to a nearby integer and returns the result as a long integer
lround(x) 	Rounds x to the nearest integer and returns the result as a long integer
modf(x, y) 	Returns the decimal part of x and writes the integer part to the memory at the pointer y
nan(s) 	Returns a NaN (Not a Number) value
nearbyint(x) 	Returns x rounded to a nearby integer
nextafter(x, y) 	Returns the closest floating point number to x in the direction of y
nexttoward(x, y) 	Returns the closest floating point number to x in the direction of y
remainder(x, y) 	Return the remainder of x/y rounded to the nearest integer
remquo(x, y, z) 	Calculates x/y rounded to the nearest integer, writes the result to the memory at the pointer z and returns the remainder.
rint(x) 	Returns x rounded to a nearby integer
round(x) 	Returns x rounded to the nearest integer
scalbln(x, y) 	Returns x*Ry (R is usually 2)
scalbn(x, y) 	Returns x*Ry (R is usually 2)

tgamma(x) 	Returns the value of the gamma function at x
trunc(x) 	Returns the integer part of x

*/



#include <stdio.h>
#include <math.h>

#define PI 3.14159265

// Define operation functions
double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a, double b) { return a * b; }
double divide(double a, double b) { return (b != 0) ? a / b : 0; }
double power(double a, double b) { return pow(a, b); }
double root(double a) { return sqrt(a); }
double sine(double deg) { return sin(deg * PI / 180); }
double cosine(double deg) { return cos(deg * PI / 180); }
double tangent(double deg) { return tan(deg * PI / 180); }
double nat_log(double x) { return log(x); }
double log10_base(double x) { return log10(x); }
double exponential(double x) { return exp(x); }
double cube_root(double x) { return cbrt(x); }

int main() {
    int choice;
    double x, y;

    do {
        printf("\n--- Scientific Calculator (Functions) ---\n");
        printf("1.Add 2.Subtract 3.Multiply 4.Divide 5.Power\n");
        printf("6.Sqrt 7.Sin 8.Cos 9.Tan 10.ln 11.Log10\n");
        printf("12.Exp 13.Abs 14.Cube Root 15.Mod 16.Round 17.Floor 18.Ceil 19.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice >= 6 && choice <= 14 || choice >= 16 && choice <= 18) {
            printf("Enter one number: ");
            scanf("%lf", &x);
        } else if (choice >= 1 && choice <= 5 || choice == 15) {
            printf("Enter two numbers: ");
            scanf("%lf %lf", &x, &y);
        }

        switch (choice) {
            case 1: printf("Result: %.4lf\n", add(x, y)); break;
            case 2: printf("Result: %.4lf\n", sub(x, y)); break;
            case 3: printf("Result: %.4lf\n", mul(x, y)); break;
            case 4: 
                if (y != 0) printf("Result: %.4lf\n", divide(x, y));
                else printf("Error: Division by zero.\n");
                break;
            case 5: printf("Result: %.4lf\n", power(x, y)); break;
            case 6: printf("Result: %.4lf\n", root(x)); break;
            case 7: printf("Result: %.4lf\n", sine(x)); break;
            case 8: printf("Result: %.4lf\n", cosine(x)); break;
            case 9: printf("Result: %.4lf\n", tangent(x)); break;
            case 10: printf("Result: %.4lf\n", nat_log(x)); break;
            case 11: printf("Result: %.4lf\n", log10_base(x)); break;
            case 12: printf("Result: %.4lf\n", exponential(x)); break;
            case 13: printf("Result: %.4lf\n", absolute(x)); break;
            case 14: printf("Result: %.4lf\n", cube_root(x)); break;
            case 15: printf("Result: %.4lf\n", modulus(x, y)); break;
            case 16: printf("Result: %.4lf\n", round_val(x)); break;
            case 17: printf("Result: %.4lf\n", floor_val(x)); break;
            case 18: printf("Result: %.4lf\n", ceil_val(x)); break;
            case 19: printf("Exiting calculator.\n"); return 0;
            default: printf("Invalid choice.\n");
        }

    } while (1);

    return 0;
}