// 3 dimensional vectors that use the fixed point arithmetic primitives described in fixap.h
#include "fixap_vec3d.h"

// vector scalar multiplication
vec_t vec3d_mul_scalar(vec_t v, fix16_t a) {
   vec_t r;
   r.x = fixap_mul(v.x, a);
   r.y = fixap_mul(v.y, a);
   r.z = fixap_mul(v.z, a);
   return r;
}


