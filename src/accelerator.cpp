#include "accelerator.h"

#if !defined(TEST_ACCELERATOR_DT10) && !defined(ACCELERATOR_TIMING_TEST)
#include "accelerators/acc_force_v4.hpp"
#endif

#if defined(TEST_ACCELERATOR_DT10) || defined(ACCELERATOR_TIMING_TEST)
//Force update to be performed by an accelerator
return_message accelerator(update_message* m) {

    return_message force;

    float r_ij_dist = newt_sqrt(m->r_ij_dist_sq); // Only square root for distance once it's known these beads interact
    // This could be any form of sqrt, and will change it upon decision of this

    float r_ij_x = m->i_pos_x - m->j_pos_x;
    float r_ij_y = m->i_pos_y - m->j_pos_y;
    float r_ij_z = m->i_pos_z - m->j_pos_z;

    float v_ij_x = m->i_vel_x - m->j_vel_x;
    float v_ij_y = m->i_vel_y - m->j_vel_y;
    float v_ij_z = m->i_vel_z - m->j_vel_z;

    // switching function
    float w_d_temp = (1.0 - r_ij_dist);
    float w_d = w_d_temp*w_d_temp;

    //Conservative force: Equation 8.5 in the dl_meso manual
    float con = (m->a_ij * (1 - (r_ij_dist/m->r_c))) / r_ij_dist;
    force.x = r_ij_x * con;
    force.y = r_ij_y * con;
    force.z = r_ij_z * con;

    // Drag force
    float vel_dot = (r_ij_x * v_ij_x) + (r_ij_y * v_ij_y) + (r_ij_z * v_ij_z);
    // float drag = w_d * vel_dot * (-1.0 * m->drag_coef);
    float drag = (w_d * vel_dot * (-1.0 * m->drag_coef)) / m->r_ij_dist_sq;
    float drag_force_x = (r_ij_x * drag);
    float drag_force_y = (r_ij_y * drag);
    float drag_force_z = (r_ij_z * drag);

    force.x = force.x + drag_force_x;
    force.y = force.y + drag_force_y;
    force.z = force.z + drag_force_z;

    // get the pairwise random number
    // dt10's hash based random num gen
    uint32_t pairwise_rand = (m->i_id ^ m->grand)*m->j_id + (m->j_id ^ m->grand)*m->i_id;
    float r_t = ((pairwise_rand / (m->rand_max_div_2)));
    float r = (r_t - 1.0) * 0.5;
    float w_r = (1.0 - r_ij_dist);

    // random force
    float ran = (m->sqrt_dt * r * w_r * m->sigma_ij) / r_ij_dist;
    float ran_force_x = r_ij_x * ran;
    float ran_force_y = r_ij_y * ran;
    float ran_force_z = r_ij_z * ran;
    force.x = force.x - ran_force_x;
    force.y = force.y - ran_force_y;
    force.z = force.z - ran_force_z;

    return force;
}
#else
    return_message accelerator(update_message* m) { return acc_force_v4(m); }
    float sqrtf (float x) { return newt_sqrt(x); }
#endif

// Converts from non-accelerated force_update to accelerated version, then back. Allows for working with DPD
update_message force_update(float i_pos_x, float i_pos_y, float i_pos_z, float j_pos_x, float j_pos_y, float j_pos_z,
                            float i_vel_x, float i_vel_y, float i_vel_z, float j_vel_x, float j_vel_y, float j_vel_z,
                            uint32_t i_id, uint32_t j_id, float sq_dist, float r_c, float a_ij, uint32_t grand) {

    // return_message force;
    // force.x = 0;
    // force.y = 0;
    // force.z = 0;

    // if (sq_dist > (r_c * r_c)) {
    //     return force;
    // }

    update_message m;
    // Position of bead i
    m.i_pos_x = i_pos_x;
    m.i_pos_y = i_pos_y;
    m.i_pos_z = i_pos_z;
    // Position of bead j
    m.j_pos_x = j_pos_x;
    m.j_pos_y = j_pos_y;
    m.j_pos_z = j_pos_z;
    // Velocity of bead i
    m.i_vel_x = i_vel_x;
    m.i_vel_y = i_vel_y;
    m.i_vel_z = i_vel_z;
    // Velocity of bead j
    m.j_vel_x = j_vel_x;
    m.j_vel_y = j_vel_y;
    m.j_vel_z = j_vel_z;
    // ID of bead i
    m.i_id = i_id;
    // ID of bead j
    m.j_id = j_id;
    // Square of euclidian distance
    m.r_ij_dist_sq = sq_dist;
    // Cutoff radius
    m.r_c = r_c;
    // Interaction between bead type of i and bead type of j
    m.a_ij = a_ij;
    // Drag constant
    m.drag_coef = 4.5;
    // Sigma constant used in random force
    m.sigma_ij = 160.0;
    // Randomly generated number
    m.grand = grand;
    // RAND_MAX / 2
    m.rand_max_div_2 = (float)4294967295 / 2;
    // Sqaure rtoot of timestep (0.02)
    m.sqrt_dt = 0.1414;

    // #ifndef TEST_ACCELERATOR_DT10
    //     force = acc_force_v4(&m);
    // #else
    //     force = accelerator(&m);
    // #endif

    return m;
}