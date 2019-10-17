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

#if defined(VISUALISE) || defined(TESTING)
#define EMIT 4
#endif

#define EMIT_COMPLETE 5

#ifdef TIMER
    #define START 6
#endif

#define END 7

#if defined(TESTING) || defined(TIMER) || defined(STATS)
#define TEST_LENGTH 1000
#endif

typedef float ptype;

// ------------------------- SIMULATION PARAMETERS --------------------------------------

// const float problem_size = 18.0; // total size of the sim universe in one dimension
// const unsigned N = 18; // the size of the sim universe in each dimension

const ptype r_c(1.0);
const ptype sq_r_c(r_c * r_c);

// interaction matrix
const ptype A[3][3] = {  {ptype(25.0), ptype(75.0), ptype(35.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0)}}; // interaction matrix

const ptype dt = 0.02; // the timestep
const ptype p_mass = 1.0; // the mass of all beads (not currently configurable per bead)

/* DT10: Playing with bonds.
    Particles of any species are bonded if:
    - They have an id with the MSB set; and
    - Their id differs by exactly 1.
    This allows for dimers and polymers, but does not allow for multi-way stuff
    In principle the bonds could break, which is a bit worrying. If they drift
    within sight again then they will re-capture, but that is unlikely.
*/
// const ptype bond_kappa=100; // Bond interaction is very strong
// const ptype bond_r0=0.5; // Distance of 0.5 to avoid escaping

//  inline bool are_beads_bonded(uint32_t a, uint32_t b)
// {
//     return (a&b&0x80000000ul) && (((a-b)==1) || ((b-a)==1));
// }

#ifdef VISUALISE
const uint32_t emitperiod = 0;
#endif

// ---------------------------------------------------------------------------------------

typedef uint32_t bead_class_t; // the type of the bead, we are not expecting too many
typedef uint32_t bead_id_t; // the ID for the bead

// defines a bead type
typedef struct _bead_t {
    bead_id_t id;
    bead_class_t type;
    Vector3D<ptype> pos;
    Vector3D<ptype> velo;
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

// Format of message
struct DPDMessage {
    uint8_t type;
    uint32_t timestep; // the timestep this message is from
// #ifdef TIMER
    uint32_t total_beads; //Used for sending cycle counts
// #endif
    unit_t from; // the unit that this message is from
    bead_t beads[1]; // the beads payload from this unit
};

// the state of the DPD Device
struct DPDState {
    uint32_t numBeads;
    uint8_t error;
    uint8_t err_mode;
    uint32_t err_timestep;
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t N;
    unit_t loc; // the location of this cube
    uint32_t bslot; // a bitmap of which bead slot is occupied
    uint32_t sentslot; // a bitmap of which bead slot has not been sent from yet
    uint32_t newBeadMap;
    bead_t newBeads[MAX_BEADS];
    uint16_t num_beads; // the number of beads in this device
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
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

#ifdef TIMER
    uint32_t board_startU;
    uint32_t board_start;
    uint32_t dpd_startU;
    uint32_t dpd_start;
    uint32_t dpd_endU;
    uint32_t dpd_end;
    uint8_t timer;
    uint32_t upperCount;
    uint32_t wraps; // Number of times tinselCycleCountU has reset
#endif
    uint8_t update_completes_received;
    uint8_t migration_completes_received;
    uint8_t emit_completes_received;
    bool first_update;
    bool first_migration;
    uint32_t total_update_beads;
    uint32_t total_migration_beads;

};

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

    // ----------------- bead slots ---------------------------
    // helper functions for managing bead slots
    inline uint32_t clear_slot(uint32_t slotlist, uint8_t pos){  return slotlist & ~(1 << pos);  }
    inline uint32_t set_slot(uint32_t slotlist, uint8_t pos){ return slotlist | (1 << pos); }

// #ifdef TIMER
    __attribute__((noinline)) uint32_t get_next_slot(uint32_t slotlist){
// #else
    // inline uint32_t get_next_slot(uint32_t slotlist){
// #endif
        uint32_t mask = 0x1;
        for(int i=0; i<MAX_BEADS; i++) {
            if(slotlist & mask){
                    return i;
            }
            mask = mask << 1; // shift to the next pos
        }
        return 0xFFFFFFFF; // we are empty
    }

    __attribute__((noinline)) uint32_t get_next_free_slot(uint32_t slotlist){
        uint32_t mask = 0x1;
        for(int i=0; i<MAX_BEADS; i++){
                if(!(slotlist & mask)) {
                       return i;
                }
                mask = mask << 1;
        }
        return 0xFFFFFFFF; // error there are no free slots!
    }

    // get the number of beads occupying a slot
    __attribute__((noinline)) uint32_t get_num_beads(uint32_t slotlist){
        uint32_t cnt = 0;
        uint32_t mask = 0x1;
        for(int i=0; i<MAX_BEADS; i++){
                if(slotlist & mask) {
                      cnt++;
                }
                mask = mask << 1;
        }
        return cnt; // error there are no free slots!
    }
    // --------------------------------------------------------

    // dt10's random number generator
	__attribute__((noinline)) uint32_t rand() {
            uint32_t c = (s->rngstate)>>32, x=(s->rngstate)&0xFFFFFFFF;
	    s->rngstate = x*((uint64_t)429488355U) + c;
	    return x^c;
	}

    // dt10's hash based random num gen
    __attribute__((noinline)) uint32_t pairwise_rand(uint32_t pid1, uint32_t pid2){
        // uint32_t la= MIN(pid1, pid2);
        // uint32_t lb= MAX(pid1, pid2);
        uint32_t s0 = (pid1 ^ s->grand)*pid2;
        uint32_t s1 = (pid2 ^ s->grand)*pid1;
        return s0 + s1;
    }

#ifndef ACCELERATE
    // calculate a new force acting between two particles
// #if defined(TESTING) || defined(STATS)
    __attribute__((noinline)) Vector3D<ptype> force_update(bead_t *a, bead_t *b){
// #else
//     Vector3D<ptype> force_update(bead_t *a, bead_t *b){
// #endif

        ptype r_ij_dist_sq = a->pos.sq_dist(b->pos);

        Vector3D<ptype> force(0.0,0.0,0.0); // accumulate the force here

        if (r_ij_dist_sq > sq_r_c) {
            return force;
        }

        ptype r_ij_dist = newt_sqrt(r_ij_dist_sq); // Only square root for distance once it's known these beads interact

        ptype a_ij = A[a->type][b->type];
        // Vector3D<ptype> r_ij = r_i - r_j;
        Vector3D<ptype> r_ij = a->pos - b->pos;
        // Vector3D<ptype> v_i = a->velo;
        // Vector3D<ptype> v_j = b->velo;
        // Vector3D<ptype> v_ij = v_i - v_j;
        Vector3D<ptype> v_ij = a->velo - b->velo;
        const ptype drag_coef(4.5); // the drag coefficient
        const ptype sigma_ij(160.0); // sqrt(2*drag_coef*KBt) assumed same for all
        const ptype sqrt_dt(0.1414); // sqrt(0.02)

        // switching function
        ptype w_d = (ptype(1.0) - r_ij_dist)*(ptype(1.0) - r_ij_dist);

        //Conservative force: Equation 8.5 in the dl_meso manual
        ptype con = a_ij * (ptype(1.0) - (r_ij_dist/r_c));
        force = (r_ij/r_ij_dist) * con;

        // Drag force
        ptype drag = w_d * r_ij.dot(v_ij) * (ptype(-1.0) * drag_coef);
        force = force + ((r_ij / (r_ij_dist_sq)) * drag);

        // get the pairwise random number
        //ptype r((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX)) * 0.5);
        ptype r_t((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX/2)));
        ptype r = (r_t - ptype(1.0)) * 0.5;
        ptype w_r = (ptype(1.0) - r_ij_dist);

        // random force
        //force = (r_ij / r_ij_dist)*sqrt_dt*r*w_r*sigma_ij*ptype(-1.0);
        ptype ran = sqrt_dt*r*w_r*sigma_ij;
        force = force - ((r_ij / r_ij_dist) * ran);

        // if(are_beads_bonded(a->id, b->id)) {
        //     force = force - (r_ij / r_ij_dist) * bond_kappa * (r_ij_dist-bond_r0);
        // }

        return force;
    }
#endif

    __attribute__((noinline)) void local_calcs(uint32_t ci) {
        uint32_t j = s->bslot;
        while(j) {
            uint32_t cj = get_next_slot(j);
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

    __attribute__((noinline)) void update_complete() {
        s->update_completes_received = 0;
        s->first_update = false;
        s->timestep++;
    #if defined(TIMER) || defined(STATS)
        // Timed run has ended
        if (s->timestep >= TEST_LENGTH) {
        #ifndef STATS
            s->dpd_endU = tinselCycleCountU();
            s->dpd_end  = tinselCycleCount();
        #endif
            *readyToSend = No;
            s->mode = END;
            return;
        }
    #endif
        s->grand = rand(); // advance the random number
        uint32_t i = s->bslot;
        while(i){
            int ci = get_next_slot(i);

            // ------ velocity verlet ------
        // #ifdef TESTING
            Vector3D<ptype> force = s->force_slot[ci].fixedToFloat();
        // #else
            // Vector3D<ptype> force = s->force_slot[ci];
        // #endif
            Vector3D<ptype> acceleration = force / p_mass;
            Vector3D<ptype> delta_v = acceleration * dt;
            // update velocity
            s->bead_slot[ci].velo = s->bead_slot[ci].velo + delta_v;
            // update position
            s->bead_slot[ci].pos = s->bead_slot[ci].pos + s->bead_slot[ci].velo*dt + acceleration*ptype(0.5)*dt*dt;

            // ----- clear the forces ---------------
        // #ifdef TESTING
            s->force_slot[ci].clear();
        // #else
            // s->force_slot[ci].set(ptype(0.0), ptype(0.0), ptype(0.0));
        // #endif

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
            }
            i = clear_slot(i, ci);
        }
        *readyToSend = Pin(0);
        if (s->migrateslot == 0) {
            s->mode = MIGRATION_COMPLETE;
        } else {
            s->mode = MIGRATION;
        }
    }// we have finished updating -- now we want to migrate

    __attribute__((noinline)) void migration_complete() {
// we have just finished a particle migration step
        s->migration_completes_received = 0;
        s->first_migration = false;
        // Add new beads to bead_slot and update bslot
        while (s->newBeadMap) {
            uint32_t ci = get_next_free_slot(s->bslot); // I hope we have space...
            uint32_t n = get_next_slot(s->newBeadMap);
            if (ci == 0xFFFFFFFF) {
                s->lost_beads++;
            } else {
                s->bslot = set_slot(s->bslot, ci);

                // welcome the new little bead
                s->bead_slot[ci].type = s->newBeads[n].type;
                s->bead_slot[ci].id = s->newBeads[n].id;
                s->bead_slot[ci].pos.set(s->newBeads[n].pos.x(), s->newBeads[n].pos.y(), s->newBeads[n].pos.z());
                s->bead_slot[ci].velo.set(s->newBeads[n].velo.x(), s->newBeads[n].velo.y(), s->newBeads[n].velo.z());
            }
            s->newBeadMap = clear_slot(s->newBeadMap, n);
        }
    #ifdef VISUALISE
        if(s->emitcnt >= emitperiod) {
            s->mode = EMIT;
            if(s->bslot) {
                s->sentslot = s->bslot;
                *readyToSend = HostPin;
            } else {
                *readyToSend = Pin(0);
                s->mode = EMIT_COMPLETE;
            }
            s->emitcnt = 0;
        } else {
            s->emitcnt++;
            s->mode = EMIT_COMPLETE;
            *readyToSend = Pin(0);
            // s->sentslot = s->bslot;
            // if(s->bslot == 0){
            //     s->mode = UPDATE_COMPLETE;
            // }
        }
        return;
    #elif defined(TESTING)
        if (s->timestep >= TEST_LENGTH) {
            s->mode = EMIT;
            if(s->bslot) {
                s->sentslot = s->bslot;
                *readyToSend = HostPin;
            } else {
                s->mode = END;
                *readyToSend = No;
            }
        } else {
            s->mode = EMIT_COMPLETE;
            *readyToSend = Pin(0);
            // s->sentslot = s->bslot;
            // if(s->bslot == 0){
            //     s->mode = UPDATE_COMPLETE;
            // }
        }
        return;
    #else
        s->mode = EMIT_COMPLETE;
        *readyToSend = Pin(0);
        // s->sentslot = s->bslot;
        // if(s->bslot == 0){
        //     s->mode = UPDATE_COMPLETE;
        // }
        return;
    #endif
    }

#if defined(VISUALISE) || defined(TESTING)
    __attribute__((noinline)) void emit_complete() {
        s->emit_completes_received = 0;
    #if defined(TESTING) || defined(STATS)
        if (s->timestep >= TEST_LENGTH) {
            s->mode = END;
            *readyToSend = No;
            return;
        }
    #endif
        s->mode = UPDATE;
        *readyToSend = Pin(0);
        s->sentslot = s->bslot;
        if(s->bslot == 0){
            s->mode = UPDATE_COMPLETE;
        }
        return;
    }
#endif

	// init handler -- called once by POLite at the start of execution
	inline void init() {
        s->numBeads = get_num_beads(s->bslot);
    #ifdef TIMER
        s->mode = START;
        if (s->timer)
            *readyToSend = HostPin;
        else
            *readyToSend = No;
        s->timestep = 0;
        s->rngstate = 1234; // start with a seed
        s->grand = rand();
        s->sentslot = s->bslot;
    #else
		s->rngstate = 1234; // start with a seed
		s->grand = rand();
		s->sentslot = s->bslot;
    #ifdef VISUALISE
		s->emitcnt = emitperiod;
    #endif
		s->mode = UPDATE;
        *readyToSend = Pin(0);
		if(s->bslot == 0) {
		    s->mode = UPDATE_COMPLETE;
        }
    #endif
	}

	// idle handler -- called once the system is idle with messages
	inline bool step() {
        // default case
        *readyToSend = No;
    #ifdef TIMER
        if (s->mode == START) {
            s->mode = UPDATE;
            *readyToSend = Pin(0);
            if(s->bslot == 0) {
                s->mode = UPDATE_COMPLETE;
            }
            s->dpd_startU = tinselCycleCountU();
            s->dpd_start  = tinselCycleCount();
            return true;
        } else if (s->timer) {
            uint32_t count = tinselCycleCountU();
            if (count < s->upperCount) {
                s->wraps++;
            }
            s->upperCount = count;
        }
    #endif
        // if (s->mode == END) {
        //     return false;
        // }
        return false;
    }

	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
        msg->type = 0x00;
        msg->timestep = s->timestep;
	    if(s->mode == UPDATE) {
            msg->type = s->mode;
            if (!s->first_update) {
                msg->total_beads = get_num_beads(s->bslot);
                s->first_update = true;
            } else {
                msg->total_beads = 0;
            }
	        uint32_t ci = get_next_slot(s->sentslot);
            local_calcs(ci);
	        // send all of our beads to neighbours
	        msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].type = s->bead_slot[ci].type;
            msg->beads[0].id = s->bead_slot[ci].id;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());

	        s->sentslot = clear_slot(s->sentslot, ci);
            *readyToSend = Pin(0);
	        if(!s->sentslot) {
                s->mode = UPDATE_COMPLETE;
	        }
	        return;
	    }

        if(s->mode == UPDATE_COMPLETE) {
            msg->type = 0xCC;
            s->update_completes_received++;
            if (s->update_completes_received == 27 && s->total_update_beads == 0) {
                update_complete();
            } else {
                *readyToSend = No;
            }
            return;
        }

	    if(s->mode == MIGRATION) { // we are in the MIGRATION mode we want to send beads to our neighbours
	        // overload from with the dst filtering will happen on the recv side
            msg->type = MIGRATION;
            if (!s->first_migration) {
                msg->total_beads = get_num_beads(s->migrateslot);
                s->first_migration = true;
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
	        s->sentslot = s->bslot;
            *readyToSend = Pin(0);
	        if(s->migrateslot == 0) {
                s->mode = MIGRATION_COMPLETE;
	        }
	        return;
	    }

        if (s->mode == MIGRATION_COMPLETE) {
            msg->type = 0xDD;
            s->migration_completes_received++;
            if (s->migration_completes_received == 27 && s->total_migration_beads == 0) {
                migration_complete();
            } else {
                *readyToSend = No;
            }
            return;
        }

	    // we are emitting our state to the host
    #if defined(VISUALISE) || defined(TESTING)
	    if(s->mode==EMIT) {
	        // we are sending a host message
	        uint32_t ci = get_next_slot(s->sentslot);

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
	        }
            return;
        #elif defined(TESTING)
            if (s->sentslot != 0) {
                *readyToSend = HostPin;
            } else {
                s->mode = END;
                *readyToSend = No;
            }
        #endif
	    }

        if (s->mode==EMIT_COMPLETE) {
            // Tell neighbours I have finished emitting.
            msg->type = 0xEE;
            s->emit_completes_received++;
            if (s->emit_completes_received == 27) {
                emit_complete();
            } else {
                *readyToSend = No;
            }
            return;
        }
    #endif

    #ifdef TIMER
        if (s->timer) {
            msg->type = 0xAB;
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->timestep = tinselCycleCountU();
            msg->total_beads = tinselCycleCount();
            *readyToSend = No;
            return;
        }
    #endif
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

	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge) {
        if (msg->timestep == s->timestep || msg->timestep == s->timestep - 1 || msg->timestep == s->timestep + 1) {
            // UPDATE MESSAGE
            if (msg->type == UPDATE) {
                // Allowed modes for this message
                // UPDATE and UPDATE_COMPLETE are expected.
                // EMIT and EMIT_COMPLETE can be OK, incase this device is waiting for a slow device to finish emitting
                // EMIT and EMIT_COMPLETE will not be affected by incoming beads being used for calculations
                // UPDATE AND UPDATE_COMPLETE will not affect emitting of neighbouring cells as this will not affect their position
                if (s->mode == UPDATE || s->mode == UPDATE_COMPLETE || s->mode == EMIT || s->mode == EMIT_COMPLETE) {
                    s->total_update_beads += msg->total_beads;
                    bead_t b;
                    b.id = msg->beads[0].id;
                    b.type = msg->beads[0].type;
                    b.pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                    b.velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                    // from the device locaton get the adjustments to the bead positions
                    int x_rel = period_bound_adj(msg->from.x - s->loc.x);
                    int y_rel = period_bound_adj(msg->from.y - s->loc.y);
                    int z_rel = period_bound_adj(msg->from.z - s->loc.z);

                    // relative position for this particle to this device
                    b.pos.x(b.pos.x() + ptype(x_rel));
                    b.pos.y(b.pos.y() + ptype(y_rel));
                    b.pos.z(b.pos.z() + ptype(z_rel));

                    // loop through the occupied bead slots -- update force
                    uint32_t i = s->bslot;
                    while(i) {
                        int ci = get_next_slot(i);
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

                        i = clear_slot(i, ci);
                    }
                    s->total_update_beads--;
                    if (s->total_update_beads == 0 && s->mode == UPDATE_COMPLETE && s->update_completes_received == 27) {
                        update_complete();
                    }
                } else {
                    // if (s->error == 0) {
                    //     s->error = 1;
                    //     s->err_mode = s->mode;
                    //     // s->err_timestep = msg->timestep;
                    // }
                }
            } else if (msg->type == MIGRATION) { // MIGRATION MESSAGE
                // Allowed modes for this message
                // Needs to be in MIGRATION or MIGRATION_COMPLETE mode ONLY
                // If in UPDATE or UPDATE_COMPLETE mode, it's possible the new bead could be factored into calculations which is bad
                // If in EMIT mode, it is possible that this bead will not be emitted as expected
                if (s->mode == MIGRATION || s->mode == MIGRATION_COMPLETE || s->mode == UPDATE_COMPLETE) {
                    // we are in the MIGRATION mode beads we receive here _may_ be added to our state
                    // when we receive a message it _may_ contain a bead that we need to add to our state
                    // it depends on whether the from address matches our own
                    s->total_migration_beads += msg->total_beads;
                    if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
                        // looks like we are getting a new addition to our family
                        uint32_t ni = get_next_free_slot(s->newBeadMap); // I hope we have space...
                        if (ni == 0xFFFFFFFF) {
                            s->lost_beads++;
                        } else {
                            s->newBeadMap = set_slot(s->newBeadMap, ni);

                            // store the new little bead
                            s->newBeads[ni].type = msg->beads[0].type;
                            s->newBeads[ni].id = msg->beads[0].id;
                            s->newBeads[ni].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                            s->newBeads[ni].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                        }
                    }
                    s->total_migration_beads--;
                    if (s->total_migration_beads == 0 && s->mode == MIGRATION_COMPLETE && s->migration_completes_received == 27) {
                        migration_complete();
                    }
                } else {
                    // if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
                    //     if (s->error == 0) {
                    //         s->error = 2;
                    //         s->err_mode = s->mode;
                    //     }
                    // }
                }
            } else if (msg->type == 0xCC) { // UPDATE_COMPLETE MESSAGE
                // Allowed modes for this message type
                // UPDATE and UPDATE_COMPLETE are expected.
                // EMIT and EMIT_COMPLETE are allowed as a neighbouring bead calculation will not affect the positions of this bead
                if (s->mode == UPDATE || s->mode == UPDATE_COMPLETE || s->mode == EMIT || s->mode == EMIT_COMPLETE) {
                    s->update_completes_received++;
                    if (s->update_completes_received == 27 && s->mode == UPDATE_COMPLETE && s->total_update_beads == 0) {
                        update_complete();
                    }
                    return;
                } else {
                    // if (s->error == 0) {
                    //     s->error = 3;
                    //     s->err_mode = s->mode;
                    // }
                }
            } else if (msg->type == 0xDD) { // MIGRATION_COMPLETE MESSAGE
                // Allowed modes
                if (s->mode == MIGRATION || s->mode == MIGRATION_COMPLETE || s->mode == UPDATE_COMPLETE) {
                    s->migration_completes_received++;
                    if (s->migration_completes_received == 27 && s->mode == MIGRATION_COMPLETE && s->total_migration_beads == 0) {
                        migration_complete();
                    }
                    return;
                } else {
                    // if (s->error == 0) {
                    //     s->error = 4;
                    //     s->err_mode = s->mode;
                    // }
                }
            } else if (msg->type == 0xEE) { // EMIT_COMPLETE MESSAGE
                // Allowed modes
                if (s->mode == EMIT || s->mode == EMIT_COMPLETE || s->mode == MIGRATION_COMPLETE) {
                    s->emit_completes_received++;
                    if (s->emit_completes_received == 27 && s->mode == EMIT_COMPLETE) {
                        emit_complete();
                    }
                } else {
                    // if (s->error == 0) {
                    //     s->error = 5;
                    //     s->err_mode = s->mode;
                    // }
                }
            } else {
                // if (s->error == 0) {
                //     s->error = 6;
                //     s->err_mode = s->mode;
                // }
            }
        } else {
            // if (s->error == 0) {
            //     s->error = 7;
            //     s->err_mode = s->mode;
            //     // s->err_timestep = msg->timestep;
            // }
        }





     //    if(s->mode == UPDATE || s->mode == UPDATE_COMPLETE) {
     //        if (msg->type == 0xCC) {
     //            // s->error = 0;
     //            s->update_completes_received++;
     //            if (s->update_completes_received == 27 && s->mode == UPDATE_COMPLETE) {
     //                update_complete();
     //            }
     //            return;
     //        } else if (msg->type == 0xDD) {
     //            // s->migration_completes_received++;
     //            if (s->error == 0)
     //                s->error = 1;
     //            return;
     //        } else if (msg->type == 0xBB) {
     //            if (s->error == 0)
     //                s->error = 2;
     //            return;
     //        } else if (msg->type == 0xAA) {
     //            // s->error = 0;
     //        } else if (msg->type == 0xEE) {
     //            if (s->error == 0)
     //                s->error = 3;
     //            // s->emit_completes_received++;
     //            return;
     //        }
     //        bead_t b;
     //        b.id = msg->beads[0].id;
     //        b.type = msg->beads[0].type;
     //        b.pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
     //        b.velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
	    //     // from the device locaton get the adjustments to the bead positions
     //        int x_rel = period_bound_adj(msg->from.x - s->loc.x);
     //        int y_rel = period_bound_adj(msg->from.y - s->loc.y);
     //        int z_rel = period_bound_adj(msg->from.z - s->loc.z);

	    //     // relative position for this particle to this device
	    //     b.pos.x(b.pos.x() + ptype(x_rel));
	    //     b.pos.y(b.pos.y() + ptype(y_rel));
	    //     b.pos.z(b.pos.z() + ptype(z_rel));

     //        // loop through the occupied bead slots -- update force
	    //     uint32_t i = s->bslot;
	    //     while(i) {
     //            int ci = get_next_slot(i);
     //        #ifndef ACCELERATE
     //            Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b);
     //        #else
     //            return_message r = force_update(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z(),
     //                                             b.pos.x(), b.pos.y(), b.pos.z(),
     //                                             s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z(),
     //                                             b.velo.x(), b.velo.y(), b.velo.z(),
     //                                             s->bead_slot[ci].id, b.id,
     //                                             s->bead_slot[ci].pos.sq_dist(b.pos),
     //                                             r_c, A[s->bead_slot[ci].type][b.type], s->grand);
     //            Vector3D<ptype> f;
     //            f.set(r.x, r.y, r.z);
     //        #endif

     //            Vector3D<int32_t> x = f.floatToFixed();
     //            s->force_slot[ci] = s->force_slot[ci] + x;

	    //         i = clear_slot(i, ci);
	    //     }
     //    #ifndef ONE_BY_ONE
     //        if (s->sentslot == 0) {
     //            local_calcs();
     //        }
     //    #endif
	    // } else if (s->mode == MIGRATION || s->mode == MIGRATION_COMPLETE) {
     //        if (msg->type == 0xDD) {
     //            // s->error = 0;
     //            s->migration_completes_received++;
     //            if (s->migration_completes_received == 27 && s->mode == MIGRATION_COMPLETE) {
     //                migration_complete();
     //            }
     //            return;
     //        } else if (msg->type == 0xCC) {
     //            if (s->error == 0)
     //                s->error = 4;
     //            // s->update_completes_received++;
     //            return;
     //        } else if (msg->type == 0xAA) {
     //            if (s->error == 0)
     //                s->error = 5;
     //            return;
     //        } else if (msg->type == 0xBB) {
     //            // s->error = 0;
     //        } else if (msg->type == 0xEE) {
     //            if (s->error == 0)
     //                s->error = 6;
     //            s->emit_completes_received++;
     //            return;
     //        }
     //        // we are in the MIGRATION mode beads we receive here _may_ be added to our state
	    //     // when we receive a message it _may_ contain a bead that we need to add to our state
	    //     // it depends on whether the from address matches our own
	    //     if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
	    //         // looks like we are getting a new addition to our family
	    //         uint32_t ci = get_next_free_slot(s->bslot); // I hope we have space...
     //            if (ci == 0xFFFFFFFF) {
     //                s->lost_beads++;
     //            } else {
     //                s->bslot = set_slot(s->bslot, ci);
    	//             s->sentslot = s->bslot;

    	//             // welcome the new little bead
    	//             s->bead_slot[ci].type = msg->beads[0].type;
    	//             s->bead_slot[ci].id = msg->beads[0].id;
    	//             s->bead_slot[ci].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
    	//             s->bead_slot[ci].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
     //            }
	    //     }
     //    } else if (s->mode == EMIT || s->mode == EMIT_COMPLETE) {
     //        if (msg->type == 0xEE) {
     //            // s->error = 0;
     //            s->emit_completes_received++;
     //            if (s->emit_completes_received == 27 && s->mode == EMIT_COMPLETE) {
     //                emit_complete();
     //            }
     //        } else if (msg->type == 0xAA) {
     //            if (s->error == 0)
     //                s->error = 7;
     //            return;
     //        } else if (msg->type == 0xBB) {
     //            if (s->error == 0)
     //                s->error = 8;
     //            return;
     //        } else if (msg->type == 0xCC) {
     //            if (s->error == 0)
     //                s->error = 9;
     //            // s->update_completes_received++;
     //            return;
     //        } else if (msg->type == 0xDD) {
     //            if (s->error == 0)
     //                s->error = 10;
     //            // s->migration_completes_received++;
     //            return;
     //        }
     //    }
	}

	// finish -- sends a message to the host on termination
	inline bool finish(volatile DPDMessage* msg) {
    #if defined(TESTING) || defined(STATS)
        msg->type = 0xAA;
        msg->timestep = s->numBeads;
        msg->total_beads = s->lost_beads;
    #endif

    #ifdef TIMER
        if (s->timer)
            msg->type = 0xAC;
        else
            msg->type = 0xAA;

        msg->from.x = s->loc.x;
        msg->from.y = s->loc.y;
        msg->from.z = s->loc.z;
        msg->timestep = s->dpd_startU;
        msg->total_beads = s->dpd_start;
        msg->beads[0].id = s->dpd_endU;
        msg->beads[0].type = s->dpd_end;
        msg->beads[0].pos.set((float)s->wraps, 0, 0);
    #endif

            // msg->type = s->error;
            // msg->timestep = s->err_mode;
	    return true;
    }

};

#endif /* _DPD_H */
