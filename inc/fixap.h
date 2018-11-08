// fixed point precision operators for the DPD simulation
//
// format 1 sign 2 Integer bits 13 fractional bits
#ifndef _FIX_AP_H
#define _FIX_AP_H

#include "stdint.h"
#include "stdio.h"
#include "math.h"

#define FRAC_BITS 13
#define FPScaleF (1 << FRAC_BITS) 

//#define FIXED_CONST(A,B) (uint32_t)((A<<13) + ((B + 0.00006103515625))*8192)

typedef int16_t fix16_t; 

// fixed-point conversion functions
constexpr const fix16_t float2fix_const(float a); // const compile time conversions
constexpr const float fix2float_const(fix16_t a);
fix16_t float2fix(float a); // dynamic runtime conversions
float fix2float(fix16_t a);

// fixed point multiplication
fix16_t fixap_mul(fix16_t a, fix16_t b);

// fixed point division
fix16_t fixap_div(fix16_t a, fix16_t b);

// square root
fix16_t fixap_sqrt(fix16_t a);

#endif /* _FIX_AP_H */
