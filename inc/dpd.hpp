// This contains functions which perform the DPD forces between beads and the
// Verlet integration which calculates velocity and moves beads based on the
// calculated forces.
// This is set up to be separate from the synchronisation found in sync.h and
// gals.h, as both of these use the same functions for calculating forces.

#ifndef DPD_H_
#define DPD_H_

#include <cstdint>

#include "DPDStructs.hpp"
#include "BeadMap.hpp"
#ifdef SERIAL
#include <iostream>
#include <map>
#endif

/********************* DEFINITIONS **************************/

#define DT10_RAND_MAX 4294967295

/********************* DPD CONSTANTS **************************/

// Cut-off radius
const ptype r_c(1.0);

// Square of cutoff radius (to reduce number of sqrt)
const ptype sq_r_c(r_c * r_c);

// Interaction matrix for water and two types of oil
#ifndef VESICLE_SELF_ASSEMBLY
const ptype A[3][3] = {  {ptype(25.0), ptype(75.0), ptype(35.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0)}
                      };
#else
const ptype A[4][4] = {  {ptype(25.0), ptype(50.0), ptype(20.0), ptype(50.0)},
                         {ptype(50.0), ptype(25.0), ptype(75.0), ptype(25.0)},
                         {ptype(20.0), ptype(75.0), ptype(25.0), ptype(75.0)},
                         {ptype(50.0), ptype(25.0), ptype(75.0), ptype(25.0)}
                      };
#endif

// Drag coefficient
const ptype drag_coef = 4.5;

// Random coefficient. Related to drag_coef and temperature - sigma = sqrt(2 * drag_coef * temp)
const ptype sigma_ij = 3;

// Mass of all beads
// const ptype p_mass = 1.0;

// Lambda used in verlet
const ptype lambda = 0.5;

/********************* BOND CONSTANTS **************************/

#ifdef BONDS
#ifndef VESICLE_SELF_ASSEMBLY
// 0.5 is the distance aimed to be kept between two bonded beads
const ptype bond_r0 = 0.5;
// Bond_kappa is the force between two bonded beads. It is very strong
const ptype bond_kappa = 128.0;
#else
const ptype bond_r0 = 0.3;
const ptype bond_kappa = 256.0;
#endif
#endif

/********************* BOND FUNCTIONS **************************/

#ifdef BONDS
/* DT10: Playing with bonds.
    Particles of any species are bonded if:
    - They have an id with the MSB set; and
    - Their id differs by exactly 1.
    This allows for dimers and polymers, but does not allow for multi-way stuff
    In principle the bonds could break, which is a bit worrying. If they drift
    within sight again then they will re-capture, but that is unlikely.
*/

inline bool are_beads_bonded(bead_id_t a, bead_id_t b)
{
    return (a & b & 0x80000000ul) && (((a - b) == 1) || ((b - a) == 1));
}
#endif

/********************* DPD FUNCTIONS **************************/

// dt10's hash based random num gen
inline uint32_t pairwise_rand(const uint32_t pid1, const uint32_t pid2, const uint32_t grand){
    uint32_t s0 = (pid1 ^ grand)*pid2;
    uint32_t s1 = (pid2 ^ grand)*pid1;
    return s0 + s1;
}

// used to help adjust the relative positions for the periodic boundary
inline int period_bound_adj(int dim) {
    if(dim > 1) {
        return -1;
    } else if (dim < -1) {
        return 1;
    } else {
        return dim;
    }
}

inline uint32_t p_rand(uint64_t *rngstate) {
    uint32_t c = (*rngstate) >> 32;
    uint32_t x = (*rngstate) & 0xFFFFFFFF;
    *rngstate = x * ((uint64_t)429488355U) + c;
    return x^c;
}

inline Vector3D<ptype> force_update(bead_t *a, bead_t *b, const uint32_t grand, const ptype inv_sqrt_dt) {

    ptype r_ij_dist_sq = a->pos.sq_dist(b->pos);

    Vector3D<ptype> force = Vector3D<ptype>(0.0, 0.0, 0.0); // accumulate the force here

#ifndef BONDS
    if (r_ij_dist_sq < sq_r_c)
#else
    if ((r_ij_dist_sq < sq_r_c) || are_beads_bonded(a->id, b->id))
#endif
    {
        // Only square root for distance once it's known these beads interact
        ptype r_ij_dist = newt_sqrt(r_ij_dist_sq);

#if !defined(DISABLE_CONS_FORCE) || !defined(DISABLE_DRAG_FORCE) || !defined(DISABLE_RAND_FORCE)
        // Switching function - w_r is used for conservative too
        ptype w_r = (ptype(1.0) - r_ij_dist/r_c);
#endif

        // Vector difference in position
        Vector3D<ptype> r_ij = a->pos - b->pos;

        // Vector Unity scale of force acting in each dimension
        Vector3D<ptype> scale = r_ij / r_ij_dist;

#ifdef BONDS
        if (are_beads_bonded(a->id, b->id)) {
            ptype s = r_ij_dist - bond_r0;
            force = force - (scale * bond_kappa * s);
        }
        if (!(r_ij_dist < r_c)) {
            return force;
        }
#endif

        // Calculate magnitudes of all forces
        // But multiply them by vector distance at the end

        // Conservative force: Equation 8.5 in the dl_meso manual
    #ifndef DISABLE_CONS_FORCE
        // Conservative magnitude for these bead types
        ptype a_ij = A[a->type][b->type];
        // Conservative portion of the force
        ptype con = a_ij * w_r;
    #else
        ptype con = 0;
    #endif

    #ifndef DISABLE_DRAG_FORCE
        // Drag switching function is equal to random switching function squared
        ptype w_d = w_r * w_r;
        // Vector difference in velocity
        Vector3D<ptype> v_ij = a->velo - b->velo;
        // Vector distance difference and Vector velocity difference dot product
        ptype dotProd = r_ij.dot(v_ij);
        // Divide this by r_ij_dist as the equation is divided by r_ij_dist squared
        dotProd /= r_ij_dist;
        // Get the drag force
        ptype drag = ptype(-1.0) * drag_coef * w_d * dotProd;
    #else
        ptype drag = 0;
    #endif

    #ifndef DISABLE_RAND_FORCE
        // Get the pairwise random number
        uint32_t rand = pairwise_rand(a->id, b->id, grand) >> 1;
        uint32_t max = DT10_RAND_MAX >> 1;
        float test = 3.46939;
        ptype r = (float(rand) / float(max)) * test;
        r = (test/2 - r);

        // random force
        ptype ran = sigma_ij * inv_sqrt_dt * r * w_r;
    #else
        ptype ran = 0;
    #endif
        return force + (scale * (con + drag + ran));
    }
    return force;
}

#ifdef BETTER_VERLET
inline void update_velocity(bead_t *bead, Vector3D<ptype> *old_velo, const ptype dt) {
    // Update intermediate velocity - If a bead migrates, this is done when received
    bead->velo = *old_velo + bead->acc * lambda * dt;
}
#endif

#ifdef BETTER_VERLET
  #ifndef FLOAT_ONLY
inline void velocity_Verlet(bead_t *bead, Vector3D<int32_t> *f, Vector3D<ptype> *old_velo, const ptype dt) {
  #else
    inline void velocity_Verlet(bead_t *bead, Vector3D<float> *f, Vector3D<ptype> *old_velo, const ptype dt) {
  #endif
#else
  #ifndef FLOAT_ONLY
inline void velocity_Verlet(bead_t *bead, Vector3D<int32_t> *f, const ptype dt) {
  #else
inline void velocity_Verlet(bead_t *bead, Vector3D<float> *f, const ptype dt) {
  #endif
#endif

#ifndef FLOAT_ONLY
    Vector3D<ptype> force = f->fixedToFloat();
#else
    Vector3D<ptype> force;
    force.set(f->x(), f->y(), f->z());
#endif

#ifndef BETTER_VERLET
    // Vector3D<ptype> acceleration = force / p_mass;

    Vector3D<ptype> delta_v = force * dt;
    // update velocity
    bead->velo = bead->velo + delta_v;

    // update position
    bead->pos = bead->pos + bead->velo * dt + force * ptype(0.5) * dt * dt;

    // ----- clear the forces ---------------
    f->clear();
#else
    // Vector3D<ptype> force = force / p_mass;
    // ------ End of previous velocity Verlet -----
    bead->velo = *old_velo + ((force + bead->acc) * dt * ptype(0.5));
    // Store old velocity
    old_velo->set(bead->velo.x(), bead->velo.y(), bead->velo.z());
    // Store old Force
    bead->acc.set(force.x(), force.y(), force.z());

    // ------ Start of new velocity Verlet ------

    // Update position
    bead->pos = bead->pos + (bead->velo * dt) + (force * ptype(0.5) * dt * dt);

    // ----- clear the forces ---------------
    f->clear();
#endif
}

#ifdef BETTER_VERLET
inline bool migration(const uint8_t map_pos, bead_t *bead, const uint8_t cell_length, const cell_t current_cell, const uint32_t vol_len, uint16_t *migrateslot, cell_t *migrate_loc, const ptype dt, Vector3D<ptype> *old_velo) {
#else
inline bool migration(const uint8_t map_pos, bead_t *bead, const uint8_t cell_length, const cell_t current_cell, const uint32_t vol_len, uint16_t *migrateslot, cell_t *migrate_loc, const ptype dt) {
#endif
        // ----- migration code ------
    bool migrating = false; // flag that says whether this particle needs to migrate
    cell_t d_loc; // the potential destination for this bead
    //    migration in the x dim
    if (bead->pos.x() >= cell_length){
        migrating = true;
        if(current_cell.x == (vol_len-1)){
            d_loc.x = 0;
        } else {
            d_loc.x = current_cell.x + 1;
        }
        bead->pos.x(bead->pos.x() - cell_length); // make it relative to the dest
    } else if (bead->pos.x() < ptype(0.0)) {
        migrating = true;
        if(current_cell.x == 0) {
            d_loc.x = vol_len - 1;
        } else {
            d_loc.x = current_cell.x - 1;
        }
       bead->pos.x(bead->pos.x() + cell_length); // make it relative to the dest
    } else {
        d_loc.x = current_cell.x;
    }

    //    migration in the y dim
    if(bead->pos.y() >= cell_length){
        migrating = true;
        if(current_cell.y == (vol_len-1)){
            d_loc.y = 0;
        } else {
            d_loc.y = current_cell.y + 1;
        }
        bead->pos.y(bead->pos.y() - cell_length); // make it relative to the dest
    } else if (bead->pos.y() < ptype(0.0)) {
        migrating = true;
        if(current_cell.y == 0) {
            d_loc.y = vol_len - 1;
        } else {
            d_loc.y = current_cell.y - 1;
        }
        bead->pos.y(bead->pos.y() + cell_length); // make it relative to the dest
    } else {
        d_loc.y = current_cell.y;
    }

    //    migration in the z dim
    if(bead->pos.z() >= cell_length){
        migrating = true;
        if(current_cell.z == (vol_len-1)){
            d_loc.z = 0;
        } else {
            d_loc.z = current_cell.z + 1;
        }
        bead->pos.z(bead->pos.z() - cell_length); // make it relative to the dest
    } else if (bead->pos.z() < ptype(0.0)) {
        migrating = true;
        if(current_cell.z == 0) {
            d_loc.z = vol_len - 1;
        } else {
            d_loc.z = current_cell.z - 1;
        }
        bead->pos.z(bead->pos.z() + cell_length); // make it relative to the dest
    } else {
        d_loc.z = current_cell.z;
    }


    if (migrating) {
        *migrateslot = set_slot(*migrateslot, map_pos);
        *migrate_loc = d_loc; // set the destination
#ifdef BETTER_VERLET
    } else {
        update_velocity(bead, old_velo, dt);
    }
#else
    }
#endif
    return migrating;
}
#ifdef ONE_BY_ONE
  #ifndef FLOAT_ONLY
inline void local_calcs(uint8_t ci, const ptype inv_sqrt_dt, const uint16_t bslot, bead_t *beads, uint32_t grand, Vector3D<int32_t> *forces)
  #else
inline void local_calcs(uint8_t ci, const ptype inv_sqrt_dt, const uint16_t bslot, bead_t *beads, uint32_t grand, Vector3D<float> *forces)
  #endif
#else
  #ifndef FLOAT_ONLY
inline void local_calcs(const ptype inv_sqrt_dt, const uint16_t bslot, bead_t *beads, uint32_t grand, Vector3D<int32_t> *forces)
  #else
inline void local_calcs(const ptype inv_sqrt_dt, const uint16_t bslot, bead_t *beads, uint32_t grand, Vector3D<float> *forces)
  #endif
#endif
    {
    #ifndef ONE_BY_ONE
        uint16_t i = bslot;
        while (i) {
            uint8_t ci = get_next_slot(i);
    #endif
            uint16_t j = bslot;
            while(j) {
                uint8_t cj = get_next_slot(j);
                if(ci != cj) {

                #ifndef ACCELERATE
                    Vector3D<ptype> f = force_update(&beads[ci], &beads[cj], grand, inv_sqrt_dt);
                #else
                    return_message r = force_update(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z(),
                                                    s->bead_slot[cj].pos.x(), s->bead_slot[cj].pos.y(), s->bead_slot[cj].pos.z(),
                                                    s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z(),
                                                    s->bead_slot[cj].velo.x(), s->bead_slot[cj].velo.y(), s->bead_slot[cj].velo.z(),
                                                    s->bead_slot[ci].id, s->bead_slot[cj].id,
                                                    s->bead_slot[ci].pos.sq_dist(s->bead_slot[cj].pos), r_c,
                                                    A[s->bead_slot[ci].type][s->bead_slot[cj].type], s->grand);
                    Vector3D<ptype> f;
                    f.set(r.x, r.y, r.z);
                #endif
                #ifndef FLOAT_ONLY
                    Vector3D<int32_t> x = f.floatToFixed();
                    forces[ci] = forces[ci] + x;
                  #ifdef REDUCE_LOCAL_CALCS
                    forces[cj] = forces[cj] - x;
                  #endif
                #else
                    forces[ci] = forces[ci] + f;
                  #ifdef REDUCE_LOCAL_CALCS
                    forces[cj] = forces[cj] - f;
                  #endif
                #endif
                }
                j = clear_slot(j, cj);
            }
    #ifndef ONE_BY_ONE
            i = clear_slot(i, ci);
        }
    #endif
    }

// #include "../src/dpd.cpp"

inline bead_t get_relative_bead(const bead_t *in, const cell_t *this_cell, const cell_t *from_cell) {
    bead_t b;
    b.id = in->id;
    b.type = in->type;
    b.pos.set(in->pos.x(), in->pos.y(), in->pos.z());
    b.velo.set(in->velo.x(), in->velo.y(), in->velo.z());
    // from the device locaton get the adjustments to the bead positions
    int8_t x_rel = period_bound_adj(from_cell->x - this_cell->x);
    int8_t y_rel = period_bound_adj(from_cell->y - this_cell->y);
    int8_t z_rel = period_bound_adj(from_cell->z - this_cell->z);

    // relative position for this particle to this device
    b.pos.x(b.pos.x() + ptype(x_rel));
    b.pos.y(b.pos.y() + ptype(y_rel));
    b.pos.z(b.pos.z() + ptype(z_rel));

    return b;
}

#endif /* _DPD_H */
