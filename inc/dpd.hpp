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
inline uint32_t pairwise_rand(const uint32_t pid1, const uint32_t pid2, const uint32_t grand) {
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

inline Vector3D<ptype> force_update(bead_t *a, bead_t *b, DPDState *s) {

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
        uint32_t rand = pairwise_rand(a->id, b->id, s->grand) >> 1;
        uint32_t max = DT10_RAND_MAX >> 1;
        float test = 3.46939;
        ptype r = (float(rand) / float(max)) * test;
        r = (test/2 - r);

        // random force
      #ifndef SMALL_DT_EARLY
        ptype ran = sigma_ij * inv_sqrt_dt * r * w_r;
      #else
        ptype ran = sigma_ij * s->inv_sqrt_dt * r * w_r;
      #endif

    #else
        ptype ran = 0;
    #endif
        return force + (scale * (con + drag + ran));
    }
    return force;
}

#ifdef BETTER_VERLET
inline void update_velocity(uint8_t bead_index, DPDState *s) {
    // Update intermediate velocity - If a bead migrates, this is done when received
  #ifndef SMALL_DT_EARLY
    s->bead_slot[bead_index].velo = s->old_velo[bead_index] + s->bead_slot[bead_index].acc * lambda * dt;
  #else
    s->bead_slot[bead_index].velo = s->old_velo[bead_index] + s->bead_slot[bead_index].acc * lambda * s->dt;
  #endif
}
#endif

inline void velocity_Verlet(uint8_t bead_index, DPDState *s) {

#ifndef FLOAT_ONLY
    Vector3D<ptype> force = s->force_slot[bead_index].fixedToFloat();
#else
    Vector3D<ptype> force;
    force.set(s->force_slot[bead_index].x(), s->force_slot[bead_index].y(), s->force_slot[bead_index].z());
#endif

  #ifdef SMALL_DT_EARLY
    const ptype dt = s->dt;
  #endif

#ifdef GRAVITY
  if (s->bead_slot[bead_index].type == 0) {
    Vector3D<ptype> gravity = Vector3D<ptype>(0, -0.5, 0);
    force = force + gravity;
  }

  if (s->bead_slot[bead_index].type != 3) {
#endif

#ifndef BETTER_VERLET
    // Vector3D<ptype> acceleration = force / p_mass;

    Vector3D<ptype> delta_v = force * dt;

    // update velocity
    s->bead_slot[bead_index].velo = s->bead_slot[bead_index].velo + delta_v;

    // update position

    s->bead_slot[bead_index].pos = s->bead_slot[bead_index].pos + s->bead_slot[bead_index].velo * dt + force * ptype(0.5) * dt * dt;

#else
    // Vector3D<ptype> force = force / p_mass;
    // ------ End of previous velocity Verlet -----
    s->bead_slot[bead_index].velo = s->old_velo[bead_index] + ((force + s->bead_slot[bead_index].acc) * dt * ptype(0.5));
    // Store old velocity
    s->old_velo[bead_index].set(s->bead_slot[bead_index].velo.x(), s->bead_slot[bead_index].velo.y(), s->bead_slot[bead_index].velo.z());
    // Store old Force
    s->bead_slot[bead_index].acc.set(force.x(), force.y(), force.z());

    // ------ Start of new velocity Verlet ------
    // Update position
    s->bead_slot[bead_index].pos = s->bead_slot[bead_index].pos + (s->bead_slot[bead_index].velo * dt) + (force * ptype(0.5) * dt * dt);

#endif

#ifdef GRAVITY
  }
#endif
    // ----- clear the forces ---------------
    s->force_slot[bead_index].clear();

}

inline bool migration(const uint8_t bead_index, DPDState *s) {

    bool migrating = false; // flag that says whether this particle needs to migrate
    cell_t d_loc; // the potential destination for this bead
    //    migration in the x dim
    if (s->bead_slot[bead_index].pos.x() >= s->cell_length){
        migrating = true;
        if(s->loc.x == (s->cells_per_dimension-1)){
            d_loc.x = 0;
        } else {
            d_loc.x = s->loc.x + 1;
        }
        s->bead_slot[bead_index].pos.x(s->bead_slot[bead_index].pos.x() - s->cell_length); // make it relative to the dest
    } else if (s->bead_slot[bead_index].pos.x() < ptype(0.0)) {
        migrating = true;
        if(s->loc.x == 0) {
            d_loc.x = s->cells_per_dimension - 1;
        } else {
            d_loc.x = s->loc.x - 1;
        }
       s->bead_slot[bead_index].pos.x(s->bead_slot[bead_index].pos.x() + s->cell_length); // make it relative to the dest
    } else {
        d_loc.x = s->loc.x;
    }

    //    migration in the y dim
    if(s->bead_slot[bead_index].pos.y() >= s->cell_length){
        migrating = true;
        if(s->loc.y == (s->cells_per_dimension-1)){
            d_loc.y = 0;
        } else {
            d_loc.y = s->loc.y + 1;
        }
        s->bead_slot[bead_index].pos.y(s->bead_slot[bead_index].pos.y() - s->cell_length); // make it relative to the dest
    } else if (s->bead_slot[bead_index].pos.y() < ptype(0.0)) {
        migrating = true;
        if(s->loc.y == 0) {
            d_loc.y = s->cells_per_dimension - 1;
        } else {
            d_loc.y = s->loc.y - 1;
        }
        s->bead_slot[bead_index].pos.y(s->bead_slot[bead_index].pos.y() + s->cell_length); // make it relative to the dest
    } else {
        d_loc.y = s->loc.y;
    }

    //    migration in the z dim
    if(s->bead_slot[bead_index].pos.z() >= s->cell_length){
        migrating = true;
        if(s->loc.z == (s->cells_per_dimension-1)){
            d_loc.z = 0;
        } else {
            d_loc.z = s->loc.z + 1;
        }
        s->bead_slot[bead_index].pos.z(s->bead_slot[bead_index].pos.z() - s->cell_length); // make it relative to the dest
    } else if (s->bead_slot[bead_index].pos.z() < ptype(0.0)) {
        migrating = true;
        if(s->loc.z == 0) {
            d_loc.z = s->cells_per_dimension - 1;
        } else {
            d_loc.z = s->loc.z - 1;
        }
        s->bead_slot[bead_index].pos.z(s->bead_slot[bead_index].pos.z() + s->cell_length); // make it relative to the dest
    } else {
        d_loc.z = s->loc.z;
    }


    if (migrating) {
        s->migrateslot = set_slot(s->migrateslot, bead_index);
        s->migrate_loc[bead_index] = d_loc; // set the destination
#ifdef BETTER_VERLET
    } else {
        update_velocity(bead_index, s);
    }
#else
    }
#endif
    return migrating;
}

#if !defined(SINGLE_FORCE_LOOP) && !defined(SEND_TO_SELF)
// Single force loop has its own local calcs
// Send to self performs local calculations when receiving a local bead like any neighbour bead
#ifdef ONE_BY_ONE
// We're interested in the forces from one bead
inline void local_calcs(uint8_t ci, const uint16_t calc_map, DPDState *s) {
#else
// Local calcs are performed in step after the update mode. At this point we
// want to (outer) loop through every bead and get the force acting on every
// other (inner) local bead.
inline void local_calcs(DPDState *s) {

        uint16_t all_bead_map = s->bslot;
        // Get all the beads this cell has
        uint16_t i = all_bead_map;
        while (i) { // While we are looping through these beads
            uint8_t ci = get_next_slot(i); // Get the index of the next bead

        #ifdef REDUCE_LOCAL_CALCS
            // Clear the slot, and then we perform calculations only on beads
            // later in the map, and subtract the force from one bead, adding
            // it to the other
            uint16_t calc_map = clear_slot(i, ci);
        #else
            // This will be all the beads not yet calculated, including ci.
            // We will loop through all of them finding ci's force on them.
            uint16_t calc_map = all_bead_map;
        #endif
#endif
            uint16_t j = calc_map;
            while(j) {
                uint8_t cj = get_next_slot(j);
              #ifndef REDUCE_LOCAL_CALCS
                if(ci != cj) {
              #endif

                #ifndef ACCELERATE
                    Vector3D<ptype> f = force_update(&s->bead_slot[ci], &s->bead_slot[cj], s);
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
                    s->force_slot[ci] = s->force_slot[ci] + x;
                  #ifdef REDUCE_LOCAL_CALCS
                    s->force_slot[cj] = s->force_slot[cj] - x;
                  #endif
                #else
                    s->force_slot[ci] = s->force_slot[ci] + f;
                  #ifdef REDUCE_LOCAL_CALCS
                    s->force_slot[cj] = s->force_slot[cj] - f;
                  #endif
                #endif
              #ifndef REDUCE_LOCAL_CALCS
                }
              #endif

                j = clear_slot(j, cj);
            }
    #ifndef ONE_BY_ONE
            i = clear_slot(i, ci);
        }
    #endif
    }
#endif

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

#ifdef SINGLE_FORCE_LOOP
 #ifdef REDUCE_LOCAL_CALCS
inline void calc_bead_force_on_beads(bead_t *acting_bead, const uint16_t bslot, DPDState *s, int8_t local_slot_position = -1) {
 #else
inline void calc_bead_force_on_beads(bead_t *acting_bead, const uint16_t bslot, DPDState *s) {
 #endif
    uint16_t i = bslot;
    while(i) {
        uint8_t ci = get_next_slot(i);

        if(acting_bead->id != s->bead_slot[ci].id) {
        #ifndef ACCELERATE
            Vector3D<ptype> f = force_update(&s->bead_slot[ci], acting_bead, s);
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
            s->force_slot[ci] = s->force_slot[ci] + x;
          #ifdef REDUCE_LOCAL_CALCS
            if (local_slot_position >= 0) {
                s->force_slot[local_slot_position] = s->force_slot[local_slot_position] - x;
            }
          #endif
        #else
            s->force_slot[ci] = s->force_slot[ci] + f;
          #ifdef REDUCE_LOCAL_CALCS
            if (local_slot_position >= 0) {
                s->force_slot[local_slot_position] = s->force_slot[local_slot_position] - f;
            }
          #endif
        #endif
        }
        i = clear_slot(i, ci);
    }
}
#endif

// #include "../src/dpd.cpp"

#endif /* _DPD_H */
