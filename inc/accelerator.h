#ifndef accelerator_h
#define accelerator_h

#include <stdint.h>
#include "utils.hpp"

// Message to be sent to the accelerator
// Includes bead information, and constants used in force calculations
typedef struct update_message {
    // Position of bead i
    float i_pos_x; // 4 bytes
    float i_pos_y; // 4 bytes
    float i_pos_z; // 4 bytes
    // Position of bead j
    float j_pos_x; // 4 bytes
    float j_pos_y; // 4 bytes
    float j_pos_z; // 4 bytes
    // Velocity of bead i
    float i_vel_x; // 4 bytes
    float i_vel_y; // 4 bytes
    float i_vel_z; // 4 bytes
    // Velocity of bead j
    float j_vel_x; // 4 bytes
    float j_vel_y; // 4 bytes
    float j_vel_z; // 4 bytes
    // ID of bead i
    uint32_t i_id; // 4 bytes
    // ID of bead j
    uint32_t j_id; // 4 bytes
    // Square of eculidian distance between beads
    float r_ij_dist_sq; // 4 bytes
    // Cutoff radius
    float r_c; // 4 bytes
    // Interaction between bead type of i and bead type of j
    float a_ij; // 4 bytes
    // Drag coefficient constant
    float drag_coef; // 4 bytes
    // Sigma constant used in random force
    float sigma_ij; // 4 bytes
    // Randomly generated number
    uint32_t grand; // 4 bytes
    // RAND_MAX / 2
    float rand_max_div_2;
    // Square root of the timestep (0.02)
    float sqrt_dt; // 4 bytes
} update_message; // Total size = 84 bytes

// What is expected to be returned from an accelerator
typedef struct return_message {
    // Force on bead i from bead j
    float x;
    float y;
    float z;
} return_message;

// #if defined(TEST_ACCELERATOR_DT10) || defined(ACCELERATOR_TIMING_TEST)
// // Force update to be performed by an accelerator
return_message accelerator(update_message* m);
// #endif

// Converts from non-accelerated force_update to accelerated version, then back. Allows for working with DPD
update_message force_update(float i_pos_x, float i_pos_y, float i_pos_z, float j_pos_x, float j_pos_y, float j_pos_z,
                             float i_vel_x, float i_vel_y, float i_vel_z, float j_vel_x, float j_vel_y, float j_vel_z,
                             uint32_t i_id, uint32_t j_id, float sq_dist, float r_c, float a_ij, uint32_t grand);

#endif
