// testing a feed-forward fixed point arithmetic function 

#include "fixed_ap.h"
#include "vec3d.h"
#include "vec3d_double.h"

//----------------------------------
// compile time generated parameters 
//----------------------------------

const float scale_f = 32;

// floating point values
const double A_dp[3][3] = {
   {25.0, 75.0, 35.0},
   {75.0, 25.0, 50.0},
   {35.0, 50.0, 25.0}
};

constexpr double drag_coef_dp = 4.5;
constexpr double dt_dp = 0.01;
constexpr double sigma_dp = sqrt(4.5*2);

// interaction matrix
constexpr fixap<int16_t,13> A[3][3] = { 
         {fixap<int16_t,13>(25.0/scale_f), fixap<int16_t,13>(75.0/scale_f), fixap<int16_t,13>(35.0/scale_f)},
         {fixap<int16_t,13>(75.0/scale_f), fixap<int16_t,13>(25.0/scale_f), fixap<int16_t,13>(50.0/scale_f)},
         {fixap<int16_t,13>(35.0/scale_f), fixap<int16_t,13>(50.0/scale_f), fixap<int16_t,13>(25.0/scale_f)} 
}; 

// K_BT = 1.0 
// R_C = 1.0
constexpr fixap<int16_t, 13> drag_coef(4.5/scale_f);
constexpr fixap<int16_t, 13> dt(0.01/scale_f);
constexpr fixap<int16_t, 13> sigma(sqrt(4.5*2)/scale_f);

// assumption r_c is 1.0
// function used to update the forces between two particles (no bonded stuff yet)
// C - the container for the type uint16_t / int16_t 
// F - the number of bits in the container allocated as fractional (the remaining bits are integer)
template<class C, unsigned F>
vec3d<C, F> forces(vec3d<C,F> a_pos, vec3d<C,F> b_pos, vec3d<C,F> a_velo, vec3d<C,F> b_velo, uint16_t a_type, uint16_t b_type){
    printf("The interaction matrix:\n");
    for(int i=0; i<3; i++) {
        printf("\t%.4f, %.4f, %.4f\n", (float)A[i][0], (float)A[i][1], (float)A[i][2]); 
    } 
    printf("drag_coef = %.4f\n", (float)drag_coef);
    printf("dt = %.4f\n", (float)dt);
    printf("sigma = %.4f\n", (float)sigma);

    // the force value we are computing
    vec3d<C,F> force(0.0, 0.0, 0.0);
    
    // common things
    fixap<C,F> r_ij_dist = a_pos.dist(b_pos); 
    vec3d<C,F> r_ij = a_pos - b_pos;

    // switching functions
    fixap<C,F> w_r = fixap<C,F>(1.0) - r_ij_dist;
    fixap<C,F> w_d = w_r * w_r; 

    // relative velocities
    vec3d<C,F> v_ij = a_velo - b_velo; 

    // force components
    //     1. conservative force
    force = force + (r_ij / r_ij_dist) * (A[a_type][b_type] * w_r); 
    
    return force;
}

// double precision version of the pairwise force update function
vec3d_double forces_dp(vec3d_double a_pos, vec3d_double b_pos, vec3d_double a_velo, vec3d_double b_velo, uint16_t a_type, uint16_t b_type) {

  // the force we are computing
  vec3d_double force(0.0,0.0,0.0);

  // common things
  double r_ij_dist = a_pos.dist(b_pos);
  vec3d_double r_ij = a_pos - b_pos;

  // switching functions
  double w_r = 1.0 - r_ij_dist;
  double w_d = w_r * w_r;

  // relative velocities
  vec3d_double v_ij = a_velo - b_velo;

  // force components
  //      1. conservative force
  force = force + (r_ij / r_ij_dist) * (A_dp[a_type][b_type]*w_r);
  //      2. drag force

  return force;
}


int main()
{
 
  const int atype = 1;
  const int btype = 2;
  constexpr vec3d<int16_t, 13> a_pos  (0.5, 0.25, 1.01);
  constexpr vec3d<int16_t, 13> b_pos  (0.85, 0.1, 1.2);
  constexpr vec3d<int16_t, 13> a_velo (0.1, 0.25, 1.2);
  constexpr vec3d<int16_t, 13> b_velo (0.15, 0.1, 0.89);

  vec3d<int16_t, 13> t_force1 = forces(a_pos, b_pos, a_velo, b_velo, atype, btype); 

  vec3d_double a_pos_dp  (0.5, 0.25, 1.01);
  vec3d_double b_pos_dp  (0.85, 0.1, 1.2);
  vec3d_double a_velo_dp (0.1, 0.25, 1.2);
  vec3d_double b_velo_dp (0.15, 0.1, 0.89);

  vec3d_double t_force_dp = forces_dp(a_pos_dp, b_pos_dp, a_velo_dp, b_velo_dp, atype, btype); 

  vec3d<int32_t, 13> wider_force = t_force1.convert_s32(); 
  printf("%s\n", t_force1.str().c_str()); 
  vec3d<int32_t, 13> scaledup_force = wider_force * scale_f;
  printf("force <dp> = %s  <16,13>  = %s\n", t_force_dp.str().c_str(), scaledup_force.str().c_str());

  return 0;
}
