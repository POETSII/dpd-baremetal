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

// vector - scalar subtraction
vec_t vec3d_div_scalar(vec_t v, fix16_t a) {
   vec_t r = {fixap_div(v.x,a), fixap_div(v.y,a), fixap_div(v.z,a)};
   return r;
}

// vector - vector addition
vec_t vec3d_add(vec_t a, vec_t b) {
    vec_t r = { a.x+b.x, a.y+b.y, a.z+b.z };
    return r;
}

// vector - vector subtraction
vec_t vec3d_sub(vec_t a, vec_t b) {
   vec_t r = {a.x - b.x, a.y - b.y, a.z - b.z}; 
   return r;
}

// vector - vector dot product
fix16_t vec3d_dot(vec_t a, vec_t b){
   return fixap_mul(a.x, b.x) + fixap_mul(a.y, b.y) + fixap_mul(a.z, b.z);     
}

// vector - vector cross product
vec_t vec3d_cross(vec_t a, vec_t b){
  vec_t r = { fixap_mul(a.y, b.z) - fixap_mul(a.z,b.y),
              fixap_mul(a.z,b.x) - fixap_mul(a.x,b.z),
              fixap_mul(a.x,b.y) - fixap_mul(a.y,b.x) }; 
  return r;
}

// vector - vector distance
// computes the euclidean distance between two vectors
fix16_t vec3d_dist(vec_t a, vec_t b){
   vec_t r = vec3d_sub(a,b);
   return vec3d_mag(r);
}

// vector magnitude
// returns the magnitude of a vector
fix16_t vec3d_mag(vec_t a){
   return fixap_sqrt(fixap_mul(a.x, a.x) + fixap_mul(a.y, a.y) + fixap_mul(a.z, a.z));
}
