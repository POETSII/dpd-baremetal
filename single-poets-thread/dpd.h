// SPDX-License-Identifier: BSD-2-Clause
// This header file is shared by the tinsel program and the host PC program

#ifndef _DPD_H_
#define _DPD_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "Vector3D.hpp"

#define MAX_BEADS 31
#define VOL_SIZE 3
#define BEAD_DENSITY 3
#define TEST_LENGTH 10000
#define DT10_RAND_MAX 4294967295

// cut-off radius
const float r_c = 1.0;
// Square of cut-off radius;
const float sq_r_c = r_c * r_c;
// interaction matrix
const float A[3][3] = {  {25.0, 75.0, 35.0},
                         {75.0, 25.0, 50.0},
                         {35.0, 50.0, 25.0}}; // interaction matrix

typedef uint32_t bead_class_t; // the type of the bead, we are not expecting too many
typedef uint32_t bead_id_t; // the ID for the bead

// defines a bead type
typedef struct _bead_t {
    bead_id_t id;
    bead_class_t type;
    Vector3D<float> pos;
    Vector3D<float> velo;
} bead_t; // 32 bytes

typedef uint16_t unit_pos_t;

// defines the unit location
struct unit_t {
    unit_pos_t x;
    unit_pos_t y;
    unit_pos_t z;

    #ifndef TINSEL // below is only needed for the host code

    // so that we can use the co-ordinate of the spatial unit as a key
    bool operator<(const unit_t& coord) const {
        if(x < coord.x) return true;
        if(x > coord.x) return false;
        //x == coord.x
        if(y < coord.y) return true;
        if(y > coord.y) return false;
        //x == coord.x && y == coord.y
        if(z < coord.z) return true;
        if(z > coord.z) return false;
        //*this == coord
        return false;
    }

    #endif /* TINSEL */

}; // 6 bytes

// defines a bead type
typedef struct _cell_t {
    unit_t loc;
    uint32_t bslot; // a bitmap of which bead slot is occupied
    bead_t bead_slot[MAX_BEADS]; // Array of beads
    Vector3D<int32_t> force_slot[MAX_BEADS]; // Force for each bead, matching bitmap
} cell_t; // 32 bytes

// Format of messages sent to host
struct HostMsg {
    uint8_t type; // Used for denoting information about this message 1 byte
    uint32_t timestep; // the timestep this message is from           4 bytes
    unit_t from; // the unit that this message is from                6 bytes
    bead_t beads[1]; // the beads payload from this unit              32 bytes
};                                                         // 43 bytes

// cell_t cells[VOL_SIZE][VOL_SIZE][VOL_SIZE];

#endif
