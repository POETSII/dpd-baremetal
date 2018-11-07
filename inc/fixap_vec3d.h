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

// scalar operations
vec_t vec3d_mul_scalar(vec_t v, fix16_t a); // computes v * a where a is a scalar
vec_t vec3d_add_scalar(vec_t v, fix16_t a); // computes v + a where a is a scalar
vec_t vec3d_sub_scalar(vec_t v, fix16_t a); // computes v - a where a is a scalar
vec_t vec3d_div_scalar(vec_t v, fix16_t a); // computes v / a where a is a scalar

// vector - vector operations
vec_t vec3d_add(vec_t a, vec_t b); // computes a + b where a and b are vectors
vec_t vec3d_sub(vec_t a, vec_t b); // computes a - b where a and b are vectors
fix16_t vec3d_dot(vec_t a, vec_t b); // computes the dot product of a and b TODO: I am worried about overflow here
vec_t vec3d_cross(vec_t a, vec_t b); // computes the cross product of a and b TODO: overflow concerns
fix16_t vec3d_dist(vec_t a, vec_t b); // computes the distance between vector a and vector b

// vector operations
fix16_t vec3d_mag(vec_t a); // computes the magnitude of the vector a

#endif /* _FIX_VEC_H */
