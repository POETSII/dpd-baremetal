// Implementation of the fixed point precision arithmetic operators
#include "fixap.h"

// multiplies two fix16_t numbers together in 1sign 2.13 format
fix16_t fixap_mul(fix16_t a, fix16_t b){
   return  ((int32_t)a * (int32_t)b) / FPScaleF;
}

// division of two fix16_t numbers
fix16_t fixap_div(fix16_t a, fix16_t b){
   return ((int32_t)a * FPScaleF)/b;
}
