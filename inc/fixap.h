// fixed point precision operators for the DPD simulation
//
// format 1 sign 2 Integer bits 13 fractional bits
#ifndef _FIX_AP_H
#define _FIX_AP_H

#include "stdint.h"

#define FPScaleF (1 << 13) 

typedef int16_t fix16_t; 

// fixed point multiplication
fix16_t fixap_mul(fix16_t a, fix16_t b);

// fixed point division
fix16_t fixap_div(fix16_t a, fix16_t b);


#endif /* _FIX_AP_H */
