// Implementation of the fixed point precision arithmetic operators
#include "fixap.h"

// conversion functions
constexpr const fix16_t float2fix_const(float a){
    return round(a * FPScaleF);
}

constexpr const float fix2float_const(fix16_t a){
   return ((float)a/FPScaleF);
}

fix16_t float2fix(float a){
    return round(a * FPScaleF);
}

float fix2float(fix16_t a){
   return ((float)a/FPScaleF);
}


// multiplies two fix16_t numbers together in 1sign 2.13 format
fix16_t fixap_mul(fix16_t a, fix16_t b){
   return  ((int32_t)a * (int32_t)b) / FPScaleF;
}

// division of two fix16_t numbers
fix16_t fixap_div(fix16_t a, fix16_t b){
   return ((int32_t)a * FPScaleF)/b;
}


// square root -- Ken Turkowski method 
// current square root is adapted from: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.178.3957&rep=rep1&type=pdf
fix16_t fixap_sqrt(fix16_t a) {

  // negative number check
  if((a & (1 << 16))) {
    printf("Error! are you attempting to square root a negative number?\n");  
    exit(-1);
  }
  
 register uint16_t root, remHi, remLo, testDiv, count;
 fix16_t val = (a << 1);

 root = 0;       /* Clear root */
 remHi = 0;      /* Clear high part of partial remainder */
 remLo = val;     /* Get argument into low part of partial remainder */
 count = 14;     /* Load loop counter */

 do {
     remHi = (remHi << 2) | (remLo >> 14); remLo <<= 2;  /* get 2 bits of arg */
     root <<= 1; /* Get ready for the next bit in the root */
     testDiv = (root << 1) + 1;  /* Test divisor */
     if (remHi >= testDiv) {
         remHi -= testDiv;
         root += 1;
     }
 } while (count-- != 0);

 return((root >> 1));
}
