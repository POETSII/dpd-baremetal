// 3 dimensional vectors that use the fixed point arithmetic primitives described in fixap.h
#ifndef _FIX_VEC_H
#define _FIX_VEC_H

#include "fixap.h" 

// basic fixed point vector type 
typedef struct _vec_t {
   fix16_t x;
   fix16_t y;
   fix16_t z;
} vec_t;

vec_t vec3d_mul_scalar(vec_t v, fix16_t a); // computes v * a where a is a scalar

#endif /* _FIX_VEC_H */
