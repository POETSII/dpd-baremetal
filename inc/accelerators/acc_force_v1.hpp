#ifndef acc_force_v1_hpp
#define acc_force_v1_hpp

#include "../accelerator.h"

return_message acc_force_v1(const update_message* m) {

    return_message force;

    float r_ij_dist = newt_sqrt(m->r_ij_dist_sq); // Only square root for distance once it's known these beads interact

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
    float con = m->a_ij * (1 - (r_ij_dist/m->r_c));
    float con_div_dist = con / r_ij_dist;
    force.x = r_ij_x * con_div_dist;
    force.y = r_ij_y * con_div_dist;
    force.z = r_ij_z * con_div_dist;

    // Drag force
    float vel_dot = (r_ij_x * v_ij_x) + (r_ij_y * v_ij_y) + (r_ij_z * v_ij_z);
    // float drag = w_d * vel_dot * (-1.0 * m->drag_coef);
    float drag = w_d * vel_dot * (-1.0 * m->drag_coef);
    float drag_div_dist_sqr = drag / m->r_ij_dist_sq;

    force.x += r_ij_x * drag_div_dist_sqr;
    force.y += r_ij_y * drag_div_dist_sqr;
    force.z += r_ij_z * drag_div_dist_sqr;

    // get the pairwise random number
    // dt10's hash based random num gen
    uint32_t pairwise_rand = (m->i_id ^ m->grand)*m->j_id + (m->j_id ^ m->grand)*m->i_id;
    float r_t = ((pairwise_rand / (m->rand_max_div_2)));
    float r = (r_t - 1.0) * 0.5;
    float w_r = (1.0 - r_ij_dist);

    // random force
    float ran = m->sqrt_dt * r * w_r * m->sigma_ij;
    float ran_div_dist = ran / r_ij_dist;
    force.x = force.x - r_ij_x * ran_div_dist;
    force.y = force.y - r_ij_y * ran_div_dist;
    force.z = force.z - r_ij_z * ran_div_dist;

    return force;
}

#endif