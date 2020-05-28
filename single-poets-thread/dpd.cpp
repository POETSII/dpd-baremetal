// SPDX-License-Identifier: BSD-2-Clause

#include <tinsel.h>
#include "dpd.h"

inline uint32_t get_next_free_slot(uint32_t slotlist) {
    uint32_t mask = 0x1;
    for(int i=0; i<31; i++){
        if(!(slotlist & mask)) {
           return i;
        }
        mask = mask << 1;
    }
    return 0xFFFFFFFF; // error there are no free slots!
}

inline uint32_t set_slot(uint32_t slotlist, uint32_t pos){ return slotlist | (1 << pos); }

inline uint32_t get_next_slot(uint32_t slotlist) {
    uint32_t mask = 0x1;
    for(int i=0; i<31; i++) {
        if(slotlist & mask) {
            return i;
        }
        mask = mask << 1; // shift to the next pos
    }
    return 0xFFFFFFFF; // we are empty
}

inline uint32_t clear_slot(uint32_t slotlist, uint32_t pos){  return slotlist & ~(1 << pos); }

inline unit_t get_neighbour_unit(unit_t current_cell, int16_t n_x, int16_t n_y, int16_t n_z) {
    unit_t neighbour;
    // X DIMENSION
    if (n_x == -1) { // If neighbour is negative
        if (current_cell.x == 0) { // If this neighbour is on min boundary
            neighbour.x = VOL_SIZE - 1; // This neighbour is on the max boundary
        } else { // Cell is not on a boundary or on max boundary
            neighbour.x = current_cell.x - 1;
        }
    } else if (n_x == 0) { // If neighbour is the same, the value is the same
        neighbour.x = current_cell.x;
    } else if (n_x == 1) { // If neighbour is positive
        if (current_cell.x == (VOL_SIZE - 1)) { // If neighbour is on max boundary
            neighbour.x = 0; // This neighbour is on the min boundary
        } else { // Cell is not on a boundary or is on min boundary
            neighbour.x = current_cell.x + 1;
        }
    }
    // Y DIMENSION
    if (n_y == -1) { // If neighbour is negative
        if (current_cell.y == 0) { // If this neighbour is on min boundary
            neighbour.y = VOL_SIZE - 1; // This neighbour is on the max boundary
        } else { // Cell is not on a boundary or on max boundary
            neighbour.y = current_cell.y - 1;
        }
    } else if (n_y == 0) { // If neighbour is the same, the value is the same
        neighbour.y = current_cell.y;
    } else if (n_y == 1) { // If neighbour is positive
        if (current_cell.y == (VOL_SIZE - 1)) { // If neighbour is on max boundary
            neighbour.y = 0; // This neighbour is on the min boundary
        } else { // Cell is not on a boundary or is on min boundary
            neighbour.y = current_cell.y + 1;
        }
    }
    // Z DIMENSION
    if (n_z == -1) { // If neighbour is negative
        if (current_cell.z == 0) { // If this neighbour is on min boundary
            neighbour.z = VOL_SIZE - 1; // This neighbour is on the max boundary
        } else { // Cell is not on a boundary or on max boundary
            neighbour.z = current_cell.z - 1;
        }
    } else if (n_z == 0) { // If neighbour is the same, the value is the same
        neighbour.z = current_cell.z;
    } else if (n_z == 1) { // If neighbour is positive
        if (current_cell.z == (VOL_SIZE - 1)) { // If neighbour is on max boundary
            neighbour.z = 0; // This neighbour is on the min boundary
        } else { // Cell is not on a boundary or is on min boundary
            neighbour.z = current_cell.z + 1;
        }
    }

    return neighbour;
}

// dt10's random number generator
inline uint32_t dt10_rand(uint64_t* rngstate) {
    uint32_t c = (*rngstate)>>32, x=(*rngstate)&0xFFFFFFFF;
    *rngstate = x*((uint64_t)429488355U) + c;
    return x^c;
}

// dt10's hash based random num gen
inline uint32_t pairwise_rand(uint32_t pid1, uint32_t pid2, uint32_t grand){
    uint32_t s0 = (pid1 ^ grand)*pid2;
    uint32_t s1 = (pid2 ^ grand)*pid1;
    return s0 + s1;
}

Vector3D<float> force_update(bead_t *a, bead_t *b, uint32_t grand) {

    float r_ij_dist_sq = a->pos.sq_dist(b->pos);

    Vector3D<float> force(0.0,0.0,0.0); // accumulate the force here

    if (r_ij_dist_sq > sq_r_c) {
        return force;
    }

    float r_ij_dist = newt_sqrt(r_ij_dist_sq); // Only square root for distance once it's known these beads interact

    float a_ij = A[a->type][b->type];
    Vector3D<float> r_ij = a->pos - b->pos;
    Vector3D<float> v_ij = a->velo - b->velo;
    const float drag_coef(4.5); // the drag coefficient
    const float sigma_ij(160.0); // sqrt(2*drag_coef*KBt) assumed same for all
    const float sqrt_dt(0.1414); // sqrt(0.02)

    // switching function
    float w_d = (1.0 - r_ij_dist)*(1.0 - r_ij_dist);

    //Conservative force: Equation 8.5 in the dl_meso manual
    float con = a_ij * (1.0 - (r_ij_dist/r_c));
    force = (r_ij/r_ij_dist) * con;

    // Drag force
    float drag = w_d * r_ij.dot(v_ij) * (-1.0 * drag_coef);
    force = force + ((r_ij / (r_ij_dist_sq)) * drag);

    // get the pairwise random number
    float r_t((pairwise_rand(a->id, b->id, grand) / (float)(DT10_RAND_MAX/2)));
    float r = (r_t - 1.0) * 0.5;
    float w_r = (1.0 - r_ij_dist);

    // random force
    float ran = sqrt_dt*r*w_r*sigma_ij;
    force = force - ((r_ij / r_ij_dist) * ran);

    // if(are_beads_bonded(a->id, b->id)) {
    //     force = force - (r_ij / r_ij_dist) * bond_kappa * (r_ij_dist-bond_r0);
    // }

    return force;
}

int main()
{
    uint64_t rngstate = 1234; // Random seed
    uint32_t grand = dt10_rand(&rngstate); // the global random number at this timestep

    cell_t cells[VOL_SIZE][VOL_SIZE][VOL_SIZE];
    tinselSetLen(3);
// INIT
    // Get message slot
    volatile HostMsg* msg = (HostMsg*) tinselSlot(15);
    // Send to host to end
    uint32_t hostId = tinselHostId();
    // Initialise cell locations
    for (uint16_t x = 0; x < VOL_SIZE; x++) {
        for (uint16_t y = 0; y < VOL_SIZE; y++) {
            for (uint16_t z = 0; z < VOL_SIZE; z++) {
                cells[x][y][z].loc = {x, y, z};
                cells[x][y][z].bslot = 0;
                cells[x][y][z].migrateslot = 0;
                for (int i = 0; i < MAX_BEADS; i++) {
                    // Clear bead slots
                    cells[x][y][z].bead_slot[i].id = 0;
                    cells[x][y][z].bead_slot[i].type = 0;
                    cells[x][y][z].bead_slot[i].pos.x(0);
                    cells[x][y][z].bead_slot[i].pos.y(0);
                    cells[x][y][z].bead_slot[i].pos.z(0);
                    cells[x][y][z].bead_slot[i].velo.x(0);
                    cells[x][y][z].bead_slot[i].velo.y(0);
                    cells[x][y][z].bead_slot[i].velo.z(0);
                    // Clear forces
                    cells[x][y][z].force_slot[i].x(0);
                    cells[x][y][z].force_slot[i].y(0);
                    cells[x][y][z].force_slot[i].z(0);
                    // Clear migrate loc
                    cells[x][y][z].migrate_loc[i].x = 0;
                    cells[x][y][z].migrate_loc[i].y = 0;
                    cells[x][y][z].migrate_loc[i].z = 0;
                }
            }
        }
    }

    // Recv beads from host

    // Get pointers to mailbox message slots
    volatile int* msgIn = (int*) tinselSlot(0);

    uint32_t beads_added = 0;
    while (true) {
        tinselAlloc(msgIn);
        tinselWaitUntil(TINSEL_CAN_RECV);
        volatile HostMsg* msg = (HostMsg*) tinselRecv();
        if (msg->type == 0xAA) {
            break;
        } else if (msg->type == 0x0A) {

            // Add bead to cells state
            // Get location (for array addressing)
            unit_t loc;
            loc.x = msg->from.x;
            loc.y = msg->from.y;
            loc.z = msg->from.z;

            // Get current bitmap
            uint32_t bslot = cells[loc.x][loc.y][loc.z].bslot;
            // Find next free slot
            uint32_t new_slot = get_next_free_slot(bslot);
            // Does it have space?
            if (new_slot == 0xFFFFFFFF) {
                return 2;
            }

            bead_t b;
            b.id = msg->beads[0].id;
            b.type = msg->beads[0].type;
            b.pos.x(msg->beads[0].pos.x());
            b.pos.y(msg->beads[0].pos.y());
            b.pos.z(msg->beads[0].pos.z());
            b.velo.x(msg->beads[0].velo.x());
            b.velo.y(msg->beads[0].velo.y());
            b.velo.z(msg->beads[0].velo.z());

            // Add it to this slot
            bslot = set_slot(bslot, new_slot);
            cells[loc.x][loc.y][loc.z].bslot = bslot;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].id = b.id;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].type = b.type;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.x(b.pos.x());
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.y(b.pos.y());
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.z(b.pos.z());

            beads_added++;
        }
    }

    // Run the actual simulation. The end time is handled later
    uint32_t timestep = 0;
    while (true) {
        // For each cell
        for (uint16_t x = 0; x < VOL_SIZE; x++) {
            for (uint16_t y = 0; y < VOL_SIZE; y++) {
                for (uint16_t z = 0; z < VOL_SIZE; z++) {
// UPDATE
                    unit_t c = {x, y, z};
                    // For each neighbour
                    for (int16_t n_x = -1; n_x <= 1; n_x++) {
                        for (int16_t n_y = -1; n_y <= 1; n_y++) {
                            for (int16_t n_z = -1; n_z <= 1; n_z++) {
                                // Find the neighbours coordinates
                                unit_t n = get_neighbour_unit(c, n_x, n_y, n_z);
                                // Bitmap of current cell
                                uint32_t current_bslot = cells[c.x][c.y][c.z].bslot;
                                // For each bead in this cell
                                while (current_bslot) {
                                    // Index of bead
                                    uint32_t ci = get_next_slot(current_bslot);
                                    // Bitmap of neighbour cell
                                    uint32_t neighbour_bslot = cells[n.x][n.y][n.z].bslot;
                                    // For each bead in neighbour cell
                                    while(neighbour_bslot) {
                                        // Index of neighbour bead
                                        uint32_t cj = get_next_slot(neighbour_bslot);
                                        // Cells calculate local interactions within here. The same bead cannot interact with itself
                                        // If the beads are not the same
                                        if(cells[c.x][c.y][c.z].bead_slot[ci].id != cells[n.x][n.y][n.z].bead_slot[cj].id) {
                                            Vector3D<float> f = force_update(&cells[c.x][c.y][c.z].bead_slot[ci], &cells[n.x][n.y][n.z].bead_slot[cj], grand);
                                            Vector3D<int32_t> x = f.floatToFixed();
                                            cells[c.x][c.y][c.z].force_slot[ci] = cells[c.x][c.y][c.z].force_slot[ci] + x;
                                        }
                                        neighbour_bslot = clear_slot(neighbour_bslot, cj);
                                    }

                                    current_bslot = clear_slot(current_bslot, ci);
                                }
                            }
                        }
                    }
                }
            }
        }
// VELOCITY VERLET
        // Increment timestep
        timestep++;
        // Timed run has ended
        if (timestep >= TEST_LENGTH) {
            break;
        }
        // Advance the random number
        grand = dt10_rand(&rngstate);

        // For each cell
        for (uint16_t x = 0; x < VOL_SIZE; x++) {
            for (uint16_t y = 0; y < VOL_SIZE; y++) {
                for (uint16_t z = 0; z < VOL_SIZE; z++) {
                    uint32_t i = cells[x][y][z].bslot;
                    // For each bead in this cell
                    while(i){
                        int ci = get_next_slot(i);

                        // ------ velocity verlet ------
                        Vector3D<float> force = cells[x][y][z].force_slot[ci].fixedToFloat();
                        Vector3D<float> acceleration = force / p_mass;
                        Vector3D<float> delta_v = acceleration * dt;
                        // update velocity
                        cells[x][y][z].bead_slot[ci].velo = cells[x][y][z].bead_slot[ci].velo + delta_v;
                        // update position
                        cells[x][y][z].bead_slot[ci].pos = cells[x][y][z].bead_slot[ci].pos + cells[x][y][z].bead_slot[ci].velo*dt + acceleration*0.5*dt*dt;

                        // ----- clear the forces ---------------
                        cells[x][y][z].force_slot[ci].set(0, 0, 0);

                        // ----- migration code ------
                        bool migrating = false; // flag that says whether this particle needs to migrate
                        unit_t d_loc; // the potential destination for this bead

                        //    migration in the x dim
                        if(cells[x][y][z].bead_slot[ci].pos.x() >= CELL_SIZE){
                            migrating = true;
                            if(cells[x][y][z].loc.x == (VOL_SIZE - 1)){
                                d_loc.x = 0;
                            } else {
                                d_loc.x = cells[x][y][z].loc.x + 1;
                            }
                            cells[x][y][z].bead_slot[ci].pos.x(cells[x][y][z].bead_slot[ci].pos.x() - CELL_SIZE); // make it relative to the dest
                        } else if (cells[x][y][z].bead_slot[ci].pos.x() < 0.0) {
                            migrating = true;
                            if(cells[x][y][z].loc.x == 0) {
                                d_loc.x = VOL_SIZE - 1;
                            } else {
                                d_loc.x = cells[x][y][z].loc.x - 1;
                            }
                           cells[x][y][z].bead_slot[ci].pos.x(cells[x][y][z].bead_slot[ci].pos.x() + CELL_SIZE); // make it relative to the dest
                        } else {
                            d_loc.x = cells[x][y][z].loc.x;
                        }

                        //    migration in the y dim
                        if(cells[x][y][z].bead_slot[ci].pos.y() >= CELL_SIZE){
                            migrating = true;
                            if(cells[x][y][z].loc.y == (VOL_SIZE - 1)){
                                d_loc.y = 0;
                            } else {
                                d_loc.y = cells[x][y][z].loc.y + 1;
                            }
                            cells[x][y][z].bead_slot[ci].pos.y(cells[x][y][z].bead_slot[ci].pos.y() - CELL_SIZE); // make it relative to the dest
                        } else if (cells[x][y][z].bead_slot[ci].pos.y() < 0.0) {
                            migrating = true;
                            if(cells[x][y][z].loc.y == 0) {
                                d_loc.y = VOL_SIZE - 1;
                            } else {
                                d_loc.y = cells[x][y][z].loc.y - 1;
                            }
                            cells[x][y][z].bead_slot[ci].pos.y(cells[x][y][z].bead_slot[ci].pos.y() + CELL_SIZE); // make it relative to the dest
                        } else {
                            d_loc.y = cells[x][y][z].loc.y;
                        }

                        //    migration in the z dim
                        if(cells[x][y][z].bead_slot[ci].pos.z() >= CELL_SIZE){
                            migrating = true;
                            if(cells[x][y][z].loc.z == (VOL_SIZE - 1)){
                                d_loc.z = 0;
                            } else {
                                d_loc.z = cells[x][y][z].loc.z + 1;
                            }
                            cells[x][y][z].bead_slot[ci].pos.z(cells[x][y][z].bead_slot[ci].pos.z() - CELL_SIZE); // make it relative to the dest
                        } else if (cells[x][y][z].bead_slot[ci].pos.z() < 0.0) {
                            migrating = true;
                            if(cells[x][y][z].loc.z == 0) {
                                d_loc.z = VOL_SIZE - 1;
                            } else {
                                d_loc.z = cells[x][y][z].loc.z - 1;
                            }
                            cells[x][y][z].bead_slot[ci].pos.z(cells[x][y][z].bead_slot[ci].pos.z() + CELL_SIZE); // make it relative to the dest
                        } else {
                            d_loc.z = cells[x][y][z].loc.z;
                        }

                        if(migrating) {
                            cells[x][y][z].migrateslot = set_slot(cells[x][y][z].migrateslot, ci);
                            cells[x][y][z].migrate_loc[ci] = d_loc; // set the destination
                        }
                        i = clear_slot(i, ci);
                    }
                }
            }
        }
// MIGRATE
        // For each cell
        for (uint16_t x = 0; x < VOL_SIZE; x++) {
            for (uint16_t y = 0; y < VOL_SIZE; y++) {
                for (uint16_t z = 0; z < VOL_SIZE; z++) {
                    // For each migrating bead in this cell
                    while (cells[x][y][z].migrateslot) {
                        uint32_t ci = get_next_slot(cells[x][y][z].migrateslot);
                        // Get destination
                        unit_t d = cells[x][y][z].migrate_loc[ci];
                        // Find space in destination
                        uint32_t di = get_next_free_slot(cells[d.x][d.y][d.z].bslot);
                        if (di != 0xFFFFFFFF) {
                            // Copy the bead data
                            cells[d.x][d.y][d.z].bead_slot[di].id = cells[x][y][z].bead_slot[ci].id;
                            cells[d.x][d.y][d.z].bead_slot[di].type = cells[x][y][z].bead_slot[ci].type;
                            cells[d.x][d.y][d.z].bead_slot[di].pos.x(cells[x][y][z].bead_slot[ci].pos.x());
                            cells[d.x][d.y][d.z].bead_slot[di].pos.y(cells[x][y][z].bead_slot[ci].pos.y());
                            cells[d.x][d.y][d.z].bead_slot[di].pos.z(cells[x][y][z].bead_slot[ci].pos.z());
                            cells[d.x][d.y][d.z].bead_slot[di].velo.x(cells[x][y][z].bead_slot[ci].velo.x());
                            cells[d.x][d.y][d.z].bead_slot[di].velo.y(cells[x][y][z].bead_slot[ci].velo.y());
                            cells[d.x][d.y][d.z].bead_slot[di].velo.z(cells[x][y][z].bead_slot[ci].velo.z());
                            // Update the destinations bitmap
                            cells[d.x][d.y][d.z].bslot = set_slot(cells[d.x][d.y][d.z].bslot, di);
                        }
                        // The bead no longer needs migrating, and doesn't belong to us
                        cells[x][y][z].migrateslot = clear_slot(cells[x][y][z].migrateslot, ci);
                        cells[x][y][z].bslot = clear_slot(cells[x][y][z].bslot, ci);
                    }
                }
            }
        }
    }

// EMIT
// Pass beads to host
    for (uint16_t x = 0; x < VOL_SIZE; x++) {
        for (uint16_t y = 0; y < VOL_SIZE; y++) {
            for (uint16_t z = 0; z < VOL_SIZE; z++) {
                uint32_t bslot = cells[x][y][z].bslot;
                while (bslot) {
                    msg->type = 0x0B;
                    msg->from.x = cells[x][y][z].loc.x;
                    msg->from.y = cells[x][y][z].loc.y;
                    msg->from.z = cells[x][y][z].loc.z;

                    uint32_t ci = get_next_slot(bslot);

                    msg->beads[0].id = cells[x][y][z].bead_slot[ci].id;
                    msg->beads[0].type = cells[x][y][z].bead_slot[ci].type;
                    msg->beads[0].pos.x(cells[x][y][z].bead_slot[ci].pos.x());
                    msg->beads[0].pos.y(cells[x][y][z].bead_slot[ci].pos.y());
                    msg->beads[0].pos.z(cells[x][y][z].bead_slot[ci].pos.z());
                    msg->beads[0].velo.x(cells[x][y][z].bead_slot[ci].velo.x());
                    msg->beads[0].velo.y(cells[x][y][z].bead_slot[ci].velo.y());
                    msg->beads[0].velo.z(cells[x][y][z].bead_slot[ci].velo.z());

                    bslot = clear_slot(bslot, ci);

                    // Wait until we can send
                    tinselWaitUntil(TINSEL_CAN_SEND);
                    // Send to host
                    tinselSend(hostId, msg);
                }
            }
        }
    }

// FINISH
    // Message to be sent to the host
    msg->type = 0xBB;

    // Wait until we can send
    tinselWaitUntil(TINSEL_CAN_SEND);

    tinselSend(hostId, msg);

  return 0;
}
