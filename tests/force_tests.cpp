// testing a feed-forward fixed point arithmetic function 

#include "fixap_vec3d.h"

// interaction matrix
constexpr fix16_t A[3][3] = { 
                              {float2fix_const(25.0/32), float2fix_const(75.0/32), float2fix_const(35.0/32)},
                              {float2fix_const(75.0/32), float2fix_const(25.0/32), float2fix_const(50.0/32)},
                              {float2fix_const(35.0/32), float2fix_const(50.0/32), float2fix_const(25.0/32)} 
                            }; 
// K_BT = 1.0 
// R_C = 1.0
constexpr fix16_t drag_coef = float2fix_const(4.5/32);
constexpr fix16_t dt = float2fix_const(0.01/32);
constexpr fix16_t sigma = float2fix_const(sqrt(4.5*2)/32);

// assumption r_c is 1.0
// function used to update the force of two particles
vec_t forces(vec_t apos, vec_t bpos, vec_t avelo, vec_t bvelo, uint16_t atype, uint8_t btype){
    printf("The interaction matrix:\n");
    for(int i=0; i<3; i++) {
        printf("\t%.4f, %.4f, %.4f\n", fix2float(A[i][0]), fix2float(A[i][1]), fix2float(A[i][2])); 
    }
    printf("drag_coef = %.8f\n", fix2float(drag_coef));
    printf("dt = %.8f\n", fix2float(dt));
    printf("sigma = %.8f\n", fix2float(sigma));

    // larger storage for the force
    vec_t force;
    
    // common things 
    fix16_t r_ij_dist = vec3d_dist(apos, bpos);
    vec_t r_ij = vec3d_sub(apos, bpos);
    
    // switching functions
    fix16_t w_r = float2fix_const(1.0) - r_ij_dist;
    fix16_t w_d = fixap_mul(w_r, w_r);
   
    // relative velocities
    vec_t v_ij = vec3d_sub(avelo, bvelo);

    // force components
    //    1. conservative 
    force = vec3d_add(force, vec3d_mul_scalar(vec3d_div_scalar(r_ij, r_ij_dist), fixap_mul(A[atype][btype], w_r))); 

}

int main()
{
 
  const int atype = 1;
  const int btype = 2;
  constexpr vec_t apos = {float2fix_const(0.5), float2fix_const(0.25), float2fix_const(1.01)};
  constexpr vec_t bpos = {float2fix_const(0.85), float2fix_const(0.1), float2fix_const(1.2)};
  constexpr vec_t avelo = {float2fix_const(0.1), float2fix_const(0.25), float2fix_const(0.16)};
  constexpr vec_t bvelo = {float2fix_const(0.15), float2fix_const(0.1), float2fix_const(0.89)};
  vec_t force = forces(apos, bpos, avelo, bvelo, atype, btype);
  printf("force value = <%.4f, %.4f, %.4f>\n", fix2float(force.x), fix2float(force.y), fix2float(force.z));

  return 0;
}
