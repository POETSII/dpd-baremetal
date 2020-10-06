// This header file is shared by the tinsel program and the host PC program
// It is used to define various DPD particles

#include <stdint.h>

// Used by POLite to count various statistics
#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#define POLITE_MAX_FANOUT 32
#include <POLite.h>

#include "Vector3D.hpp"
#ifdef ACCELERATE
    #include "accelerator.h"
#endif

#ifndef _DPD_H_
#define _DPD_H_

#define UNIT_SPACE 1.0 // a cube 1.0 x 1.0 x 1.0
#define PADDING 0

#define DT10_RAND_MAX 4294967295

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_BEADS 31

#define UPDATE 0
#define UPDATE_COMPLETE 1
#define MIGRATION 2
#define MIGRATION_COMPLETE 3
#define EMIT 4
#define EMIT_COMPLETE 5
#define START 6
#define END 7

#ifndef ONE_BY_ONE
const uint8_t NEIGHBOURS = 27;
#else
const uint8_t NEIGHBOURS = 26;
#endif

typedef float ptype;

// ------------------------- SIMULATION PARAMETERS --------------------------------------

// Cut-off radius
const ptype r_c(1.0);
// Square of cutoff radius (to reduce number of sqrt)
const ptype sq_r_c(r_c * r_c);
// Interaction matrix
const ptype A[3][3] = {  {ptype(25.0), ptype(75.0), ptype(35.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0)}};
// Timestep and inverse sqrt of timestep
#ifndef SMALL_DT_EARLY
const ptype dt = 0.02;
// Inverse square root of dt - dt^(-1/2)
const ptype inv_sqrt_dt = 7.071067812;
#else
const ptype normal_dt = 0.02;
const ptype early_dt = 0.002;
// Inverse square root of dt - dt^(-1/2)
const ptype normal_inv_sqrt_dt = 7.071067812;
const ptype early_inv_sqrt_dt = 22.360679775;
#endif

// Mass of all beads
const ptype p_mass = 1.0;
// Drag coefficient
const ptype drag_coef = 4.5;
// Temperature
// const ptype temp = 1.0;
// Random coefficient. Related to drag_coef and temperature - sigma = sqrt(2 * drag_coef * temp)
const ptype sigma_ij = 3;
// Lambda used in verlet
const ptype lambda = 0.5;

#ifdef BONDS
// Bond_kappa is the force between two bonded beads. It is very strong
const ptype bond_kappa=128;
// 0.5 is the distance aimed to be kept between two bonded beads
const ptype bond_r0=0.5;
#endif

#ifdef VISUALISE
const uint32_t emitperiod = 1;
#endif

// ----------------------------------------------------------------------------

#ifndef BETTER_VERLET
typedef uint32_t bead_class_t; // the type of the bead, we are not expecting too many
#else
typedef uint8_t bead_class_t; // the type of the bead, we are not expecting too many
#endif
typedef uint32_t bead_id_t; // the ID for the bead

// defines a bead type
typedef struct _bead_t {
    bead_id_t id;
    bead_class_t type;
    Vector3D<ptype> pos;
    Vector3D<ptype> velo;
#ifdef BETTER_VERLET
    Vector3D<ptype> acc;
#endif
} bead_t; // 32 bytes - 44 bytes with BETTER_VERLET

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

// Format of message
struct DPDMessage {
    uint8_t mode;
    uint8_t type;
    uint32_t timestep; // The timestep this message is from
    uint8_t total_beads; // Used for sending cycle counts
    unit_t from; // The cell that this message is from
    bead_t beads[1]; // The beads payload from this unit
}; // 48 bytes - 60 bytes with BETTER_VERLET

// the state of the DPD Device
struct DPDState {
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t N;
    unit_t loc; // the location of this cube
    uint32_t bslot; // a bitmap of which bead slot is occupied
    uint32_t sentslot; // a bitmap of which bead slot has not been sent from yet
    uint32_t newBeadMap;
    // bead_t newBeads[MAX_BEADS];
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint32_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    unit_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in.
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    uint32_t lost_beads;
    uint32_t max_timestep;

    uint8_t updates_received;
    uint8_t update_completes_received;
    uint8_t migrations_received;
    uint8_t migration_completes_received;
    uint8_t emit_complete_sent;
    uint8_t emit_completes_received;
    uint8_t updates_sent;
    uint8_t migrates_sent;
    int32_t total_update_beads;
    int32_t total_migration_beads;

#ifdef SMALL_DT_EARLY
    ptype dt;
    ptype inv_sqrt_dt;
#endif

#ifdef MESSAGE_MANAGEMENT
    int8_t msgs_to_recv; // Number of messages expected from neighbours. Will only send when all neighbours have sent at least one message
    uint8_t nbs_complete; // Neighbours which are not expected to send any more. Works in tandem with the above
#endif
    uint8_t error;
};

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

    // ----------------- bead slots ---------------------------
    // helper functions for managing bead slots
    inline uint32_t clear_slot(uint32_t slotlist, uint8_t pos){  return slotlist & ~(1 << pos);  }
    inline uint32_t set_slot(uint32_t slotlist, uint8_t pos){ return slotlist | (1 << pos); }

    uint8_t get_next_slot(uint32_t slotlist){

        uint32_t mask = 0x1;
        for(uint8_t i = 0; i < MAX_BEADS; i++) {
            if(slotlist & mask){
                    return i;
            }
            mask = mask << 1; // shift to the next pos
        }
        return 0xFF; // we are empty
    }

    uint8_t get_next_free_slot(uint32_t slotlist){
        uint32_t mask = 0x1;
        for(uint8_t i = 0; i < MAX_BEADS; i++){
                if(!(slotlist & mask)) {
                       return i;
                }
                mask = mask << 1;
        }
        return 0xFF; // error there are no free slots!
    }

    // get the number of beads occupying a slot
    uint8_t get_num_beads(uint32_t slotlist){
        uint8_t cnt = 0;
        uint32_t mask = 0x1;
        for(uint8_t i = 0; i < MAX_BEADS; i++){
                if(slotlist & mask) {
                      cnt++;
                }
                mask = mask << 1;
        }
        return cnt; // error there are no free slots!
    }
    // --------------------------------------------------------

    // dt10's random number generator
	uint32_t rand() {
        uint32_t c = (s->rngstate)>>32, x=(s->rngstate)&0xFFFFFFFF;
	    s->rngstate = x*((uint64_t)429488355U) + c;
	    return x^c;
	}

    // dt10's hash based random num gen
    uint32_t pairwise_rand(uint32_t pid1, uint32_t pid2){
        uint32_t s0 = (pid1 ^ s->grand)*pid2;
        uint32_t s1 = (pid2 ^ s->grand)*pid1;
        return s0 + s1;
    }

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

#ifndef ACCELERATE
    // calculate a new force acting between two particles
    // __attribute__((noinline)) Vector3D<ptype> force_update(bead_t *a, bead_t *b){
    Vector3D<ptype> force_update(bead_t *a, bead_t *b) {

        ptype r_ij_dist_sq = a->pos.sq_dist(b->pos);

        Vector3D<ptype> force = Vector3D<ptype>(0.0, 0.0, 0.0); // accumulate the force here

#ifndef BONDS
        if (r_ij_dist_sq < sq_r_c) {
#else
        if ((r_ij_dist_sq < sq_r_c) || are_beads_bonded(a->id, b->id)) {
#endif

            ptype r_ij_dist = newt_sqrt(r_ij_dist_sq); // Only square root for distance once it's known these beads interact

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
            // ptype r((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX/2)));
            const ptype test = 3.466008;
            ptype r = (pairwise_rand(a->id, b->id) / float(DT10_RAND_MAX)) * test;
            r = ((test/2) - r);

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
#endif

    __attribute__((noinline)) bool update_complete() {
        if (!(s->update_completes_received == NEIGHBOURS && s->mode == UPDATE_COMPLETE && s->total_update_beads == 0 && s->updates_received == NEIGHBOURS && s->updates_sent == 2)) {
            return false;
        }
        s->update_completes_received = 0;
        s->updates_sent = 0;
        s->updates_received = 0;

    #ifdef MESSAGE_MANAGEMENT
        s->nbs_complete = 0;
        s->msgs_to_recv = 0;
    #endif
        s->timestep++;
    #ifdef SMALL_DT_EARLY
        if (s->timestep == 1000) {
            s->dt = normal_dt;
            s->inv_sqrt_dt = normal_inv_sqrt_dt;
        }
    #endif
    #if defined(TIMER)
        // Timed run has ended
        if (s->timestep >= s->max_timestep) {
            *readyToSend = HostPin;
            s->mode = END;
            return true;
        }
    #elif defined(STATS)
        if (s->timestep >= s->max_timestep) {
            s->mode = END;
            *readyToSend = No;
            return false;
        }
    #endif
        s->grand = rand(); // advance the random number
        uint32_t i = s->bslot;
        while(i){
            uint8_t ci = get_next_slot(i);
            // ------ velocity verlet ------
        // #ifdef TESTING
            Vector3D<ptype> force = s->force_slot[ci].fixedToFloat();
        // #else
            // Vector3D<ptype> force = s->force_slot[ci];
        // #endif

#ifndef BETTER_VERLET
            Vector3D<ptype> acceleration = force / p_mass;
        #ifndef SMALL_DT_EARLY
            Vector3D<ptype> delta_v = acceleration * dt;
        #else
            Vector3D<ptype> delta_v = acceleration * s->dt;
        #endif
            // update velocity
            s->bead_slot[ci].velo = s->bead_slot[ci].velo + delta_v;
            // update position
        #ifndef SMALL_DT_EARLY
            s->bead_slot[ci].pos = s->bead_slot[ci].pos + s->bead_slot[ci].velo*dt + acceleration*ptype(0.5)*dt*dt;
        #else
            s->bead_slot[ci].pos = s->bead_slot[ci].pos + s->bead_slot[ci].velo*dt + acceleration*ptype(0.5)*s->dt*s->dt;
        #endif

            // ----- clear the forces ---------------
            s->force_slot[ci].clear();
#else
            Vector3D<ptype> new_acc = force / p_mass;
            // ------ End of previous velocity Verlet -----
        #ifndef SMALL_DT_EARLY
            s->bead_slot[ci].velo = s->old_velo[ci] + ((new_acc + s->bead_slot[ci].acc) * dt * ptype(0.5));
        #else
            s->bead_slot[ci].velo = s->old_velo[ci] + ((new_acc + s->bead_slot[ci].acc) * s->dt * ptype(0.5));
        #endif
            // Store old velocity
            s->old_velo[ci].set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());
            // Store old Force
            s->bead_slot[ci].acc.set(new_acc.x(), new_acc.y(), new_acc.z());

            // ------ Start of new velocity Verlet ------

            // Update position
        #ifndef SMALL_DT_EARLY
            s->bead_slot[ci].pos = s->bead_slot[ci].pos + (s->bead_slot[ci].velo * dt) + (new_acc * ptype(0.5) * dt * dt);
        #else
            s->bead_slot[ci].pos = s->bead_slot[ci].pos + (s->bead_slot[ci].velo * s->dt) + (new_acc * ptype(0.5) * s->dt * s->dt);
        #endif

            // ----- clear the forces ---------------
            s->force_slot[ci].set(0, 0, 0);
#endif

            // ----- migration code ------
            bool migrating = false; // flag that says whether this particle needs to migrate
            unit_t d_loc; // the potential destination for this bead

            //    migration in the x dim
            if(s->bead_slot[ci].pos.x() >= s->unit_size){
                migrating = true;
                if(s->loc.x == (s->N-1)){
                    d_loc.x = 0;
                } else {
                    d_loc.x = s->loc.x + 1;
                }
                s->bead_slot[ci].pos.x(s->bead_slot[ci].pos.x() - s->unit_size); // make it relative to the dest
            } else if (s->bead_slot[ci].pos.x() < ptype(0.0)) {
                migrating = true;
                if(s->loc.x == 0) {
                    d_loc.x = s->N - 1;
                } else {
                    d_loc.x = s->loc.x - 1;
                }
               s->bead_slot[ci].pos.x(s->bead_slot[ci].pos.x() + s->unit_size); // make it relative to the dest
            } else {
                d_loc.x = s->loc.x;
            }

            //    migration in the y dim
            if(s->bead_slot[ci].pos.y() >= s->unit_size){
                migrating = true;
                if(s->loc.y == (s->N-1)){
                    d_loc.y = 0;
                } else {
                    d_loc.y = s->loc.y + 1;
                }
                s->bead_slot[ci].pos.y(s->bead_slot[ci].pos.y() - s->unit_size); // make it relative to the dest
            } else if (s->bead_slot[ci].pos.y() < ptype(0.0)) {
                migrating = true;
                if(s->loc.y == 0) {
                    d_loc.y = s->N - 1;
                } else {
                    d_loc.y = s->loc.y - 1;
                }
                s->bead_slot[ci].pos.y(s->bead_slot[ci].pos.y() + s->unit_size); // make it relative to the dest
            } else {
                d_loc.y = s->loc.y;
            }

            //    migration in the z dim
            if(s->bead_slot[ci].pos.z() >= s->unit_size){
                migrating = true;
                if(s->loc.z == (s->N-1)){
                    d_loc.z = 0;
                } else {
                    d_loc.z = s->loc.z + 1;
                }
                s->bead_slot[ci].pos.z(s->bead_slot[ci].pos.z() - s->unit_size); // make it relative to the dest
            } else if (s->bead_slot[ci].pos.z() < ptype(0.0)) {
                migrating = true;
                if(s->loc.z == 0) {
                    d_loc.z = s->N - 1;
                } else {
                    d_loc.z = s->loc.z - 1;
                }
                s->bead_slot[ci].pos.z(s->bead_slot[ci].pos.z() + s->unit_size); // make it relative to the dest
            } else {
                d_loc.z = s->loc.z;
            }

            if(migrating) {
                s->migrateslot = set_slot(s->migrateslot, ci);
                s->migrate_loc[ci] = d_loc; // set the destination
        #ifdef BETTER_VERLET
            } else {
                // Update intermediate velocity - If a bead migrates, this is done when received
            #ifndef SMALL_DT_EARLY
                s->bead_slot[ci].velo = s->old_velo[ci] + new_acc * lambda * dt;
            #else
                s->bead_slot[ci].velo = s->old_velo[ci] + new_acc * lambda * s->dt;
            #endif
            }
        #else
            }
        #endif
            i = clear_slot(i, ci);
        }
        *readyToSend = Pin(0);
        if (s->migrateslot == 0) {
            s->mode = MIGRATION_COMPLETE;
        } else {
            s->mode = MIGRATION;
        }
        return true;
    }

    __attribute__((noinline)) bool migration_complete() {
        if (s->total_migration_beads == 0 && s->mode == MIGRATION_COMPLETE && s->migration_completes_received == NEIGHBOURS && s->migrations_received == NEIGHBOURS && s->migrates_sent == 2) {
        // we have just finished a particle migration step
            s->migration_completes_received = 0;
            s->migrations_received = 0;
            s->migrates_sent = 0;
            // Add new beads to bead_slot and update bslot
            s->bslot = s->newBeadMap;
            s->newBeadMap = 0;
        #ifdef VISUALISE
            if(s->emitcnt >= emitperiod) {
                s->mode = EMIT;
                if(s->bslot) {
                    s->sentslot = s->bslot;
                    *readyToSend = HostPin;
                } else {
                    *readyToSend = Pin(0);
                    // *readyToSend = HostPin;
                    s->mode = EMIT_COMPLETE;
                }
                s->emitcnt = 1;
            } else {
                s->emitcnt++;
                s->mode = EMIT_COMPLETE;
                *readyToSend = Pin(0);
                // *readyToSend = HostPin;
            }
            return true;
        #elif defined(TESTING)
            if (s->timestep >= s->max_timestep) {
                s->mode = EMIT;
                if(s->bslot) {
                    s->sentslot = s->bslot;
                    *readyToSend = HostPin;
                } else {
                    s->mode = END;
                    *readyToSend = HostPin;
                }
            } else {
                s->mode = EMIT_COMPLETE;
                *readyToSend = Pin(0);
                // *readyToSend = HostPin;
            }
            return true;
        #else
            s->mode = EMIT_COMPLETE;
            *readyToSend = Pin(0);
            // *readyToSend = HostPin;
            return true;
        #endif
        } else {
            return false;
        }
    }

    bool emit_complete() {
        if (s->emit_completes_received == NEIGHBOURS && s->mode == EMIT_COMPLETE && s->emit_complete_sent == 2) {
            s->emit_completes_received = 0;
            s->emit_complete_sent = 0;
        #if defined(TESTING)
            if (s->timestep >= s->max_timestep) {
                s->mode = END;
                *readyToSend = HostPin;
                return true;
            }
        #elif defined(STATS)
            if (s->timestep >= s->max_timestep) {
                s->mode = END;
                *readyToSend = No;
                return false;
            }
        #endif
            s->mode = UPDATE;
            *readyToSend = Pin(0);
            if(s->bslot == 0){
                s->newBeadMap = s->bslot;
                s->mode = UPDATE_COMPLETE;
            }
            return true;
        } else {
            return false;
        }
    }

#ifdef ONE_BY_ONE
    void local_calcs(uint8_t ci) {
        uint32_t j = s->bslot;
        while(j) {
            uint8_t cj = get_next_slot(j);
            if(ci != cj) {
                #ifndef ACCELERATE
                    Vector3D<ptype> f = force_update(&s->bead_slot[ci], &s->bead_slot[cj]);
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

                    Vector3D<int32_t> x = f.floatToFixed();
                    s->force_slot[ci] = s->force_slot[ci] + x;
            }
            j = clear_slot(j, cj);
        }
    }
#endif

	// init handler -- called once by POLite at the start of execution
	inline void init() {
		s->rngstate = 1234; // start with a seed
		s->grand = rand();
		s->mode = UPDATE;
        *readyToSend = Pin(0);
		if(s->bslot == 0) {
            s->newBeadMap = s->bslot;
		    s->mode = UPDATE_COMPLETE;
        }
	}

	// idle handler -- called once the system is idle with messages
	inline bool step() {
    #ifdef STATS
        if (s->mode == END) {
            return false;
        }
    #endif
        return true;
    }

	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
        msg->mode = 0x00;
        msg->timestep = s->timestep;
	    if(s->mode == UPDATE) {
            msg->mode = UPDATE;
        #ifdef MESSAGE_MANAGEMENT
            s->msgs_to_recv += NEIGHBOURS - s->nbs_complete; // Only expect messages from neighbours who have beads
            msg->type = 0x00; // 0 represents a standard update message
        #endif

            if (!s->updates_sent) {
                s->sentslot = s->bslot;
                msg->total_beads = get_num_beads(s->bslot);
                s->updates_sent = 1;
            } else {
                msg->total_beads = 0;
            }

	        uint8_t ci = get_next_slot(s->sentslot);

        #ifdef ONE_BY_ONE
            local_calcs(ci);
        #endif
	        // send all of our beads to neighbours
	        msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].type = s->bead_slot[ci].type;
            msg->beads[0].id = s->bead_slot[ci].id;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());

	        s->sentslot = clear_slot(s->sentslot, ci);

        #ifdef MESSAGE_MANAGEMENT
            if (!s->sentslot) {
                *readyToSend = Pin(0);
                s->newBeadMap = s->bslot;
                s->mode = UPDATE_COMPLETE;
                msg->type = 0xBC; // 0xBC represents this is my last bead.
            } else if (s->msgs_to_recv <= 0 || s->nbs_complete == NEIGHBOURS) {
                *readyToSend = Pin(0); // We have beads to send, and we've received a round of messages from all neighbours
            } else {
                *readyToSend = No; // We need to wait for a full round of messages from neighbours
            }
            return;
        #elif defined(IMPROVED_GALS)
            // The last bead is being sent, stick the final bead identifier in this message
            if (!s->sentslot) {
                *readyToSend = No;
                s->newBeadMap = s->bslot;
                s->mode = UPDATE_COMPLETE;
                s->updates_sent = 2;
                msg->type = 0xBE; // This signifies that this is the last update bead
                update_complete();
            } else {
                msg->type = 0x00;
                *readyToSend = Pin(0);
            }
            return;
        #else
            *readyToSend = Pin(0);
            if(!s->sentslot) {
                s->newBeadMap = s->bslot;
                s->mode = UPDATE_COMPLETE;
            }
            return;
        #endif
	    }

        if(s->mode == UPDATE_COMPLETE) {
            msg->mode = UPDATE_COMPLETE;
        #ifndef IMPROVED_GALS
            // This message is only sent if there's no beads to be sent for update in IMPROVED_GALS
            msg->total_beads = s->updates_sent;
        #endif
            s->updates_sent = 2;
            if (!update_complete()) {
                *readyToSend = No;
            }
            return;
        }

	    if(s->mode == MIGRATION) { // we are in the MIGRATION mode we want to send beads to our neighbours
	        // overload from with the dst filtering will happen on the recv side
            msg->mode = MIGRATION;
            if (!s->migrates_sent) {
                msg->total_beads = get_num_beads(s->migrateslot);
                s->migrates_sent = 1;
            } else {
                msg->total_beads = 0;
            }

	        uint8_t ci = get_next_slot(s->migrateslot);
            msg->from.x = s->migrate_loc[ci].x;
            msg->from.y = s->migrate_loc[ci].y;
            msg->from.z = s->migrate_loc[ci].z;
	        msg->beads[0].type = s->bead_slot[ci].type;
	        msg->beads[0].id = s->bead_slot[ci].id;
	        msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
	        msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());

	        // clear the migration slot bit
	        s->migrateslot = clear_slot(s->migrateslot, ci);
	        // clear the bead slot -- it no longer belongs to us
	        s->bslot = clear_slot(s->bslot, ci);
            // clear the new bead slot -- To match bslot
            s->newBeadMap = clear_slot(s->newBeadMap, ci);
	        // s->sentslot = s->bslot;

        #ifdef IMPROVED_GALS
            // The last bead is being sent, stick the final bead identifier in this message
            if (s->migrateslot == 0) {
                *readyToSend = No;
                s->mode = MIGRATION_COMPLETE;
                msg->type = 0xEB; // This signifies that this is the last migration bead
                s->migrates_sent = 2;
                migration_complete();
            } else {
                msg->type = 0x00;
            }
        #else
            *readyToSend = Pin(0);
	        if (s->migrateslot == 0) {
                s->mode = MIGRATION_COMPLETE;
	        }
        #endif
	        return;
	    }

        if (s->mode == MIGRATION_COMPLETE) {
            *readyToSend = No;
            msg->mode = MIGRATION_COMPLETE;
        #ifndef IMPROVED_GALS
            msg->total_beads = s->migrates_sent;
        #endif
            s->migrates_sent = 2;
            // s->migration_completes_received++;
            if (!migration_complete()) {
                *readyToSend = No;
            }
            return;
        }

	    // we are emitting our state to the host
    #if defined(VISUALISE) || defined(TESTING)
	    if(s->mode == EMIT) {
	        // we are sending a host message
	        uint8_t ci = get_next_slot(s->sentslot);
            msg->timestep = s->timestep;
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());
	        msg->beads[0].id = s->bead_slot[ci].id;
	        msg->beads[0].type = s->bead_slot[ci].type;

	        s->sentslot = clear_slot(s->sentslot, ci);
        #ifdef VISUALISE
	        if(s->sentslot != 0) {
                *readyToSend = HostPin;
	        } else {
	            s->sentslot = s->bslot;
                s->mode = EMIT_COMPLETE;
	            *readyToSend = Pin(0);
                // *readyToSend = HostPin;
	        }
            return;
        #elif defined(TESTING)
            if (s->sentslot != 0) {
                *readyToSend = HostPin;
            } else {
                s->mode = END;
                *readyToSend = HostPin;
            }
        #endif
	    }
    #endif

        if (s->mode==EMIT_COMPLETE) {
            // if (s->emit_complete_sent == 0) {
            //     msg->type = 0xDD;
            //     msg->timestep = s->timestep;
            //     s->emit_complete_sent = 1;
            //     *readyToSend = Pin(0);
            // } else {
                // Tell neighbours I have finished emitting.
                msg->mode = EMIT_COMPLETE;
                s->emit_complete_sent = 2;
                // s->emit_completes_received++;
                if (!emit_complete()) {
                    *readyToSend = No;
                }
            // }
            return;
        }

        if (s->mode == END) {
            msg->type = 0xAA;
        #ifndef BEAD_COUNTER
            msg->timestep = s->timestep;
        #else
            msg->timestep = get_num_beads(s->bslot);
        #endif
            *readyToSend = No;
            return;
        }

	}

	// used to help adjust the relative positions for the periodic boundary
	inline int8_t period_bound_adj(int8_t dim) {
        if(dim > 1) {
            return -1;
        } else if (dim < -1) {
            return 1;
        } else {
            return dim;
        }
    }

#ifdef MESSAGE_MANAGEMENT
    inline void set_rts() {
        if (s->sentslot) {
            if (s->msgs_to_recv <= 0 || s->nbs_complete == NEIGHBOURS) {
                *readyToSend = Pin(0); // We have beads to send, and we've received a round of messages from all neighbours
            }
        }
    }
#endif

	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge) {
        if (msg->timestep == s->timestep || msg->timestep == s->timestep - 1 || msg->timestep == s->timestep + 1) {
            // UPDATE MESSAGE
            if (msg->mode == UPDATE) {
                // Allowed modes for this message
                // UPDATE and UPDATE_COMPLETE are expected.
                // EMIT and EMIT_COMPLETE can be OK, incase this device is waiting for a slow device to finish emitting
                // EMIT and EMIT_COMPLETE will not be affected by incoming beads being used for calculations
                // UPDATE AND UPDATE_COMPLETE will not affect emitting of neighbouring cells as this will not affect their position
                if (s->mode == UPDATE || s->mode == UPDATE_COMPLETE || s->mode == EMIT || s->mode == EMIT_COMPLETE) {

                    if (msg->total_beads > 0) {
                        s->updates_received++;
                        s->total_update_beads += msg->total_beads;
                    }

                    bead_t b;
                    b.id = msg->beads[0].id;
                    b.type = msg->beads[0].type;
                    b.pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                    b.velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                    // from the device locaton get the adjustments to the bead positions
                    int8_t x_rel = period_bound_adj(msg->from.x - s->loc.x);
                    int8_t y_rel = period_bound_adj(msg->from.y - s->loc.y);
                    int8_t z_rel = period_bound_adj(msg->from.z - s->loc.z);

                    // relative position for this particle to this device
                    b.pos.x(b.pos.x() + ptype(x_rel));
                    b.pos.y(b.pos.y() + ptype(y_rel));
                    b.pos.z(b.pos.z() + ptype(z_rel));

                    // loop through the occupied bead slots -- update force
                    uint32_t i = s->bslot;
                    while(i) {
                        uint8_t ci = get_next_slot(i);
                    #ifndef ONE_BY_ONE
                        if(s->bead_slot[ci].id != b.id) {
                    #endif
                        #ifndef ACCELERATE
                            Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b);
                        #else
                            return_message r = force_update(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z(),
                                                             b.pos.x(), b.pos.y(), b.pos.z(),
                                                             s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z(),
                                                             b.velo.x(), b.velo.y(), b.velo.z(),
                                                             s->bead_slot[ci].id, b.id,
                                                             s->bead_slot[ci].pos.sq_dist(b.pos),
                                                             r_c, A[s->bead_slot[ci].type][b.type], s->grand);
                            Vector3D<ptype> f;
                            f.set(r.x, r.y, r.z);
                        #endif

                            Vector3D<int32_t> x = f.floatToFixed();
                            s->force_slot[ci] = s->force_slot[ci] + x;
                    #ifndef ONE_BY_ONE
                        }
                    #endif
                        i = clear_slot(i, ci);
                    }
                    s->total_update_beads--;

                #ifdef IMPROVED_GALS
                    // Last bead received from this neighbour
                    if (msg->type == 0xBE) {
                        s->update_completes_received++;
                    }
                #endif

                    update_complete();
                #ifdef MESSAGE_MANAGEMENT
                    s->msgs_to_recv--;
                    if (msg->type == 0xBC) {
                        s->nbs_complete++; // This is the senders last message, but the bead in it is useful
                    }
                    set_rts();
                #endif
                }
            } else if (msg->mode == MIGRATION) { // MIGRATION MESSAGE
                // Allowed modes for this message
                // Needs to be in MIGRATION or MIGRATION_COMPLETE mode ONLY
                // If in UPDATE or UPDATE_COMPLETE mode, it's possible the new bead could be factored into calculations which is bad
                // If in EMIT mode, it is possible that this bead will not be emitted as expected
                if (s->mode == MIGRATION || s->mode == MIGRATION_COMPLETE || s->mode == UPDATE_COMPLETE) {
                    // we are in the MIGRATION mode beads we receive here _may_ be added to our state
                    // when we receive a message it _may_ contain a bead that we need to add to our state
                    // it depends on whether the from address matches our own
                    if (msg->total_beads > 0) {
                        s->total_migration_beads += msg->total_beads;
                        s->migrations_received++;
                    }
                #ifdef IMPROVED_GALS
                    // Last bead received from this neighbour
                    if (msg->type == 0xEB) {
                        s->migration_completes_received++;
                    }
                #endif
                    if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
                        // looks like we are getting a new addition to our family
                        uint8_t ni = get_next_free_slot(s->newBeadMap);
                        if (ni == 0xFFFFFFFF) {
                            s->lost_beads++;
                        } else {
                            s->newBeadMap = set_slot(s->newBeadMap, ni);

                        #ifndef BETTER_VERLET
                            s->bead_slot[ni].type = msg->beads[0].type;
                            s->bead_slot[ni].id = msg->beads[0].id;
                            s->bead_slot[ni].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                            s->bead_slot[ni].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                            s->force_slot[ni].set(0.0, 0.0, 0.0);
                        #else
                            // Welcome the new little bead
                            s->bead_slot[ni].type = msg->beads[0].type;
                            s->bead_slot[ni].id = msg->beads[0].id;
                            s->bead_slot[ni].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                            s->bead_slot[ni].acc.set(msg->beads[0].acc.x(), msg->beads[0].acc.y(), msg->beads[0].acc.z());
                            s->force_slot[ni].set(0.0, 0.0, 0.0);

                            // Store old velocity
                            s->old_velo[ni].set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                            // Update velocity
                        #ifndef SMALL_DT_EARLY
                            s->bead_slot[ni].velo = s->old_velo[ni] + (s->bead_slot[ni].acc * lambda * dt);
                        #else
                            s->bead_slot[ni].velo = s->old_velo[ni] + (s->bead_slot[ni].acc * lambda * s->dt);
                        #endif
                        #endif
                        }
                    }
                    s->total_migration_beads--;
                    migration_complete();
                }
            } else if (msg->mode == UPDATE_COMPLETE) { // UPDATE_COMPLETE MESSAGE
                // Allowed modes for this message type
                // UPDATE and UPDATE_COMPLETE are expected.
                // EMIT and EMIT_COMPLETE are allowed as a neighbouring bead calculation will not affect the positions of this bead
                if (s->mode == UPDATE || s->mode == UPDATE_COMPLETE || s->mode == EMIT || s->mode == EMIT_COMPLETE) {
                    s->update_completes_received++;
                #ifdef IMPROVED_GALS
                    // This is only received if a cell has no beads to send for updates
                    s->updates_received++;
                #else
                    if (!msg->total_beads) {
                        s->updates_received++;
                    #ifdef MESSAGE_MANAGEMENT
                        s->msgs_to_recv--;
                        s->nbs_complete++;
                        set_rts();
                    #endif
                    }
                #endif
                    update_complete();
                    return;
                }
            } else if (msg->mode == MIGRATION_COMPLETE) { // MIGRATION_COMPLETE MESSAGE
                // Allowed modes
                if (s->mode == MIGRATION || s->mode == MIGRATION_COMPLETE || s->mode == UPDATE_COMPLETE) {
                    s->migration_completes_received++;
                #ifdef IMPROVED_GALS
                    // This is only received if a cell has no beads to send for migrations
                    s->migrations_received++;
                #else
                    if (!msg->total_beads) {
                        s->migrations_received++;
                    }
                #endif
                    migration_complete();
                    return;
                }
            } else if (msg->mode == EMIT_COMPLETE) { // EMIT_COMPLETE MESSAGE
                // Allowed modes
                if (s->mode == EMIT || s->mode == EMIT_COMPLETE || s->mode == MIGRATION_COMPLETE) {
                    s->emit_completes_received++;
                    emit_complete();
                }
            }
        }
	}

	// finish -- sends a message to the host on termination
	inline bool finish(volatile DPDMessage* msg) {
    #ifdef STATS
        msg->type = 0xAA;
    #endif
        return true;
    }

};

#endif /* _DPD_H */
