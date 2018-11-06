// 3 dimensional vectors that use the fixed point arithmetic primitives described in fixap.h
#include "fixap_vec3d.h"

// vector - scalar multiplication
vec_t vec3d_mul_scalar(vec_t v, fix16_t a) {
   vec_t r = {fixap_mul(v.x, a), fixap_mul(v.y, a), fixap_mul(v.z, a) };
   return r;
}

// vector - scalar addition
vec_t vec3d_add_scalar(vec_t v, fix16_t a) {
   vec_t r = {v.x + a, v.y + a, v.z + a};
   return r;
}

// vector - scalar subtraction
vec_t vec3d_sub_scalar(vec_t v, fix16_t a) {
   vec_t r = {v.x - a, v.y - a, v.z - a};
   return r;
}

// vector - vector addition
vec_t vec3d_add(vec_t a, vec_t b) {
    vec_t r = { a.x+b.x, a.y+b.y, a.z+b.z };
    return r;
}


