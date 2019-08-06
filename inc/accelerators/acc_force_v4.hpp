#ifndef acc_force_v4_hpp
#define acc_force_v4_hpp

#include "../accelerator.h"

#include <cassert>
#include <cstdio>
#include <cmath>

/* V4 changes:
    Refactored some calculations to keep as many lines of code as possible to a single calculation only.
    This should help make accumulation as deterministic as possible.
*/

return_message acc_force_v4(const update_message* m) {

    // return_message force;

    float r_ij_dist = sqrtf(m->r_ij_dist_sq); // Only square root for distance once it's known these beads interact

    float inv_dist = 1 / r_ij_dist;
    float inv_dist_sqr = inv_dist*inv_dist;

    float r_ij_x = m->i_pos_x - m->j_pos_x;
    float r_ij_y = m->i_pos_y - m->j_pos_y;
    float r_ij_z = m->i_pos_z - m->j_pos_z;

    float v_ij_x = m->i_vel_x - m->j_vel_x;
    float v_ij_y = m->i_vel_y - m->j_vel_y;
    float v_ij_z = m->i_vel_z - m->j_vel_z;

    // switching function
    float add_inv_dist=1-r_ij_dist;
    float w_d = add_inv_dist*add_inv_dist;

    //Conservative force: Equation 8.5 in the dl_meso manual
    #ifdef TEST_ACCELERATOR_DT10
        assert(m->r_c==1.0);
    #endif
    float con = m->a_ij * add_inv_dist;
    float con_div_dist = con * inv_dist;
    float fx_c = r_ij_x * con_div_dist;
    float fy_c = r_ij_y * con_div_dist;
    float fz_c= r_ij_z * con_div_dist;

    // Drag force
    // float vel_dot = (r_ij_x * v_ij_x) + (r_ij_y * v_ij_y) + (r_ij_z * v_ij_z);
    float vel_dot_x = r_ij_x * v_ij_x;
    float vel_dot_y = r_ij_y * v_ij_y;
    float vel_dot_z = r_ij_z * v_ij_z;
    float vel_dot = vel_dot_x + vel_dot_y;
    vel_dot = vel_dot + vel_dot_z;
    // float drag = w_d * vel_dot * (-1.0 * m->drag_coef);
    float drag = w_d * vel_dot;
    drag = drag * - m->drag_coef;
    float drag_div_dist_sqr = drag * inv_dist_sqr;

    float fx_d = r_ij_x * drag_div_dist_sqr;
    float fy_d = r_ij_y * drag_div_dist_sqr;
    float fz_d = r_ij_z * drag_div_dist_sqr;

    // get the pairwise random number
    // dt10's hash based random num gen
    // uint32_t pairwise_rand = (m->i_id ^ m->grand)*m->j_id + (m->j_id ^ m->grand)*m->i_id;
    uint32_t pairwise_rand_a = m->i_id ^ m->grand;
    pairwise_rand_a = pairwise_rand_a * m->j_id;
    uint32_t pairwise_rand_b = m->j_id ^ m->grand;
    pairwise_rand_b = pairwise_rand_b * m->i_id;
    uint32_t pairwise_rand = pairwise_rand_a + pairwise_rand_b;

    //fprintf(stderr, "  pairwise_rand = %08x, i_id=%08x, j_id=%08x\n", pairwise_rand, m->i_id, m->j_id);
    const uint32_t rand_max_div_2 = (4294967295/2);
    #ifdef TEST_ACCELERATOR_DT10
        assert(m->rand_max_div_2 == rand_max_div_2);
    #endif
    // const float inv_rand_max_div_2 = 1.0/rand_max_div_2;
    const float inv_rand_max = 1.0/4294967295;

    float r_t = float(pairwise_rand);
    float r = inv_rand_max * (r_t - rand_max_div_2);
    float w_r = add_inv_dist;

    // random force
    // float ran = r * w_r * (m->sigma_ij * m->sqrt_dt); // TODO: Should sigma_ij*sqrt_dt be pre-calculated?
    float sigma_sqrt_dt = m->sigma_ij * m->sqrt_dt;
    float ran = r * w_r;
    ran = ran * sigma_sqrt_dt;
    float ran_div_dist = - ran * inv_dist;
    float fx_r = r_ij_x * ran_div_dist;
    float fy_r = r_ij_y * ran_div_dist;
    float fz_r = r_ij_z * ran_div_dist;

    return return_message{ fx_c+fx_d+fx_r, fy_c+fy_d+fy_r, fz_c+fz_d+fz_r };
}

#endif