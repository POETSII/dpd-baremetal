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

#ifndef _DPD_H_
#define _DPD_H_

#define UNIT_SPACE 1.0 // a cube 1.0 x 1.0 x 1.0
#define PADDING 0

#define DT10_RAND_MAX 4294967295

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_BEADS 31

#define UPDATE 0
#define MIGRATION 1

#if !defined(TIMER) && !defined(STATS)
#define EMIT 2
#endif

#ifdef TIMER
    #define START 3
#endif

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

#if !defined(TIMER) && !defined(TESTING) && !defined(STATS)
const uint32_t emitperiod = 10;
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
#ifdef TIMER
    uint32_t extra; //Used for sending cycle counts
#endif
    unit_t from; // the unit that this message is from
    bead_t beads[1]; // the beads payload from this unit
};

// the state of the DPD Device
struct DPDState {
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t N;
    unit_t loc; // the location of this cube
    uint32_t bslot; // a bitmap of which bead slot is occupied
    uint32_t sentslot; // a bitmap of which bead slot has not been sent from yet
    uint32_t local_slot_i; // an outer bitmap of which bead slot has not been used in local calculations yet
    uint32_t local_slot_j; // an inner bitmap of which bead slot has not been used in local calculations yet
    uint16_t num_beads; // the number of beads in this device
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
// #ifdef TESTING
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
// #else
    // Vector3D<ptype> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
// #endif
    uint32_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    unit_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in 0 = update; 1 = migration
#if !defined(TIMER) && !defined(TESTING) && !defined(STATS)
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    // uint32_t lost_beads;

#ifdef TIMER
    uint32_t board_startU;
    uint32_t board_start;
    uint32_t dpd_startU;
    uint32_t dpd_start;
    uint32_t dpd_endU;
    uint32_t dpd_end;
    uint8_t timer;
#endif

};

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

    inline void clear_seen(uint32_t* seen) {
        for (int i = 0; i < 100; i++) {
            seen[i] = 0;
        }
    }
    // ----------------- bead slots ---------------------------
    // helper functions for managing bead slots
    inline uint32_t clear_slot(uint32_t slotlist, uint8_t pos){  return slotlist & ~(1 << pos);  }
    inline uint32_t set_slot(uint32_t slotlist, uint8_t pos){ return slotlist | (1 << pos); }
    inline bool is_slot_set(uint32_t slotlist, uint8_t pos){ return slotlist & (1 << pos); }

    inline uint32_t get_next_slot(uint32_t slotlist){
        uint32_t mask = 0x1;
        for(int i=0; i<MAX_BEADS; i++) {
            if(slotlist & mask){
                    return i;
            }
            mask = mask << 1; // shift to the next pos
        }
        return 0xFFFFFFFF; // we are empty
    }

    inline uint32_t get_next_free_slot(uint32_t slotlist){
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
    inline uint32_t get_num_beads(uint32_t slotlist){
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
	uint32_t rand() {
            uint32_t c = (s->rngstate)>>32, x=(s->rngstate)&0xFFFFFFFF;
	    s->rngstate = x*((uint64_t)429488355U) + c;
	    return x^c;
	}

    // dt10's hash based random num gen
    uint32_t pairwise_rand(uint32_t pid1, uint32_t pid2){
        // uint32_t la= MIN(pid1, pid2);
        // uint32_t lb= MAX(pid1, pid2);
        uint32_t s0 = (pid1 ^ s->grand)*pid2;
        uint32_t s1 = (pid2 ^ s->grand)*pid1;
        return s0 + s1;
    }

// OPTION 1 - Pass bead positions, bead velocities, squared euclidian distance, and all constants
    typedef struct update_message1 {
        Vector3D<float> i_pos; // Position of bead i - 12 bytes now - Aiming for 6 with int16_t
        Vector3D<float> j_pos; // Position of bead j - 12 bytes now - Aiming for 6 with int16_t
        Vector3D<float> i_vel; // Velocity of bead i - 12 bytes now - Aiming for 6 with bfloat
        Vector3D<float> j_vel; // Velocity of bead j - 12 bytes now - Aiming for 6 with bfloat
        uint32_t i_id; // ID of bead i - 4 bytes. Needs to remain at 4 bytes to allow for large numbers of beads with unique IDs
        uint32_t j_id; // ID of bead j - 4 bytes. Needs to remain at 4 bytes to allow for large numbers of beads with unique IDs
        float r_ij_dist_sq; // Square of the euclidian distance between beads - 4 bytes - Ideally remain at 4
        // Square root could be peformed in accelerator? If not, pass in the squar-root of this.
        float r_c;// Cutoff radius used to determine if calculations are performed - 4 bytes - 2 using uint16_t
        // This check should probably be done before sending anything to an accelerator, otherwise it's a waster of accelerator time.
        float a_ij; // Interaction between bead type of i and bead type of j - 4 bytes - Aiming for 2 with bfloat
        float drag_coef; // Drag coefficient constant - 4 bytes now - Aiming for 2 with bfloat
        float sigma_ij; // Sigma constant used in random force - 4 bytes now - Aiming for 2 with bfloat
        float sqrt_dt; // Square root of the timestep (0.02) - 4 bytes now - Aiming for 2 with bfloat
        // Could this be calculated in accelerator?
    } update_message1; // Total size = 80 bytes now - 46 bytes with bfloat and fixed-point positions
    // This will not require more calculations to be done in the POLite app, but could be heavy on the accelerator.
    // Testing this with a timed run is too large, as it's considerable to make the structs before calling force_update.
    // POLite reports this as taking: 32.5701 seconds
    // Make test - FAIL - Need to investigate

#if defined(TESTING) || defined(STATS)
    __attribute__((noinline)) Vector3D<ptype> force_update1(update_message1* m){
#else
    Vector3D<ptype> force_update1(update_message1* m){
#endif

      // ptype r_ij_dist = m->i_pos.sq_dist(m->j_pos);

        Vector3D<ptype> force(0.0,0.0,0.0); // accumulate the force here

        if (m->r_ij_dist_sq > (m->r_c * m->r_c)) {
            return force;
        }

        float r_ij_dist = newt_sqrt(m->r_ij_dist_sq); // Only square root for distance once it's known these beads interact

        // ptype a_ij = A[a->type][b->type];
        // Vector3D<ptype> r_ij = r_i - r_j;
        Vector3D<ptype> r_ij = m->i_pos - m->j_pos;
        // Vector3D<ptype> v_i = a->velo;
        // Vector3D<ptype> v_j = b->velo;
        // Vector3D<ptype> v_ij = v_i - v_j;
        Vector3D<ptype> v_ij = m->i_vel - m->j_vel;
        // const ptype drag_coef(4.5); // the drag coefficient
        //const ptype sigma_ij(275.0); // sqrt(2*drag_coef*KBt) assumed same for all
        // const ptype sigma_ij(160.0); // sqrt(2*drag_coef*KBt) assumed same for all
        // const ptype sqrt_dt(0.1414); // sqrt(0.02)

        // switching function
        ptype w_d = (ptype(1.0) - r_ij_dist)*(ptype(1.0) - r_ij_dist);

        //Conservative force: Equation 8.5 in the dl_meso manual
        force = (r_ij/r_ij_dist) * (m->a_ij * (ptype(1.0) - (r_ij_dist/r_c)));

        // Drag force
        force = force + (r_ij / (r_ij_dist * r_ij_dist)) * w_d * r_ij.dot(v_ij) * (ptype(-1.0) * m->drag_coef);

        // get the pairwise random number
        //ptype r((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX)) * 0.5);
        ptype r_t((pairwise_rand(m->i_id, m->j_id) / (float)(DT10_RAND_MAX/2))); // May need to pass RAND_MAX in...or hardwire it?
        ptype r = (r_t - ptype(1.0)) * 0.5;
        ptype w_r = (ptype(1.0) - r_ij_dist);

        // random force
        //force = (r_ij / r_ij_dist)*sqrt_dt*r*w_r*sigma_ij*ptype(-1.0);
        force = force - (r_ij / r_ij_dist)*m->sqrt_dt*r*w_r*m->sigma_ij;

        return force;
    }

// // OPTION 2 - Pass pre-calculated distances and velocity differences etc, as well as constants
//     typedef struct update_message2 {
//         float r_ij_dist; // Euclidian distance between beads i and j - 4 bytes now - Aiming for 2 with bfloat
//         Vector3D<float> r_ij; // Vector distance between beads i and j - 12 bytes now - Aiming for 6 with int16_t
//         Vector3D<float> v_ij; // Vector difference in velocity between beads i and j - 12 bytes now - Aiming for 6 with bfloat
//         // float r_t; // Pairwise random generated using the two bead ids - 4 bytes now - Could be 2?
//         uint32_t i_id;
//         uint32_t j_id;
//         // Square root could be peformed in accelerator? If not, pass in the squar-root of this.
//         float r_c;// Cutoff radius used to determine if calculations are performed - 4 bytes - 2 using uint16_t
//         float a_ij; // Interaction between bead type of i and bead type of j - 4 bytes - Aiming for 2 with bfloat
//         float drag_coef; // Drag coefficient constant - 4 bytes now - Aiming for 2 with bfloat
//         float sigma_ij; // Sigma constant used in random force - 4 bytes now - Aiming for 2 with bfloat
//         float sqrt_dt; // Square root of the timestep (0.02) - 4 bytes now - Aiming for 2 with bfloat
//         // Could this be calculated in accelerator?
//     } update_message2; // Total size = 48 bytes now - 30 bytes with bfloat and fixed-point positions
//     // Adds overhead, as calculations will need to be done before passing to the accelerator.
//     // Testing this with a timed run works! Reported time is: 10.880934 (Quicker than normal, there must be a problem)
//     // Make test - FAIL - potentially pairwise random being shortened
//     // Remove pairwise random, pass in IDs: make test - FAIL - More to think about...
//     //

// #if defined(TESTING) || defined(STATS)
//     __attribute__((noinline)) Vector3D<ptype> force_update2(update_message2* m){
// #else
//     Vector3D<ptype> force_update2(update_message2* m){
// #endif

//         // ptype r_ij_dist = a->pos.sq_dist(b->pos);

//         Vector3D<ptype> force(0.0,0.0,0.0); // accumulate the force here

//         // if (r_ij_dist > sq_r_c) {
//         //     return force;
//         // }

//         // r_ij_dist = newt_sqrt(r_ij_dist); // Only square root for distance once it's known these beads interact

//         // ptype a_ij = A[a->type][b->type];
//         // Vector3D<ptype> r_ij = r_i - r_j;
//         // Vector3D<ptype> r_ij = a->pos - b->pos;
//         // Vector3D<ptype> v_i = a->velo;
//         // Vector3D<ptype> v_j = b->velo;
//         // Vector3D<ptype> v_ij = v_i - v_j;
//         // Vector3D<ptype> v_ij = a->velo - b->velo;
//         // const ptype drag_coef(4.5); // the drag coefficient
//         //const ptype sigma_ij(275.0); // sqrt(2*drag_coef*KBt) assumed same for all
//         // const ptype sigma_ij(160.0); // sqrt(2*drag_coef*KBt) assumed same for all
//         // const ptype sqrt_dt(0.1414); // sqrt(0.02)

//         // switching function
//         ptype w_d = (ptype(1.0) - m->r_ij_dist)*(ptype(1.0) - m->r_ij_dist);

//         //Conservative force: Equation 8.5 in the dl_meso manual
//         force = (m->r_ij/m->r_ij_dist) * (m->a_ij * (ptype(1.0) - (m->r_ij_dist/m->r_c)));

//         // Drag force
//         force = force + (m->r_ij / (m->r_ij_dist * m->r_ij_dist)) * w_d * m->r_ij.dot(m->v_ij) * (ptype(-1.0) * m->drag_coef);

//         // get the pairwise random number
//         //ptype r((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX)) * 0.5);
//         ptype r_t((pairwise_rand(m->i_id, m->i_id) / (float)(DT10_RAND_MAX/2)));
//         ptype r = (r_t - ptype(1.0)) * 0.5;
//         ptype w_r = (ptype(1.0) - m->r_ij_dist);

//         // random force
//         //force = (r_ij / r_ij_dist)*sqrt_dt*r*w_r*sigma_ij*ptype(-1.0);
//         force = force - (m->r_ij / m->r_ij_dist)*m->sqrt_dt*r*w_r*m->sigma_ij;

//         return force;
//     }


    // calculate a new force acting between two particles
// #if defined(TESTING) || defined(STATS)
//     __attribute__((noinline)) Vector3D<ptype> force_update(bead_t *a, bead_t *b){
// #else
//     Vector3D<ptype> force_update(bead_t *a, bead_t *b){
// #endif

//         ptype r_ij_dist = a->pos.sq_dist(b->pos);

//         Vector3D<ptype> force(0.0,0.0,0.0); // accumulate the force here

//         if (r_ij_dist > sq_r_c) {
//             return force;
//         }

//         r_ij_dist = newt_sqrt(r_ij_dist); // Only square root for distance once it's known these beads interact

//         ptype a_ij = A[a->type][b->type];
//         // Vector3D<ptype> r_ij = r_i - r_j;
//         Vector3D<ptype> r_ij = a->pos - b->pos;
//         // Vector3D<ptype> v_i = a->velo;
//         // Vector3D<ptype> v_j = b->velo;
//         // Vector3D<ptype> v_ij = v_i - v_j;
//         Vector3D<ptype> v_ij = a->velo - b->velo;
//         const ptype drag_coef(4.5); // the drag coefficient
//         //const ptype sigma_ij(275.0); // sqrt(2*drag_coef*KBt) assumed same for all
//         const ptype sigma_ij(160.0); // sqrt(2*drag_coef*KBt) assumed same for all
//         const ptype sqrt_dt(0.1414); // sqrt(0.02)

//         // switching function
//         ptype w_d = (ptype(1.0) - r_ij_dist)*(ptype(1.0) - r_ij_dist);

//         //Conservative force: Equation 8.5 in the dl_meso manual
//         force = (r_ij/r_ij_dist) * (a_ij * (ptype(1.0) - (r_ij_dist/r_c)));

//         // Drag force
//         force = force + (r_ij / (r_ij_dist * r_ij_dist)) * w_d * r_ij.dot(v_ij) * (ptype(-1.0) * drag_coef);

//         // get the pairwise random number
//         //ptype r((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX)) * 0.5);
//         ptype r_t((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX/2)));
//         ptype r = (r_t - ptype(1.0)) * 0.5;
//         ptype w_r = (ptype(1.0) - r_ij_dist);

//         // random force
//         //force = (r_ij / r_ij_dist)*sqrt_dt*r*w_r*sigma_ij*ptype(-1.0);
//         force = force - (r_ij / r_ij_dist)*sqrt_dt*r*w_r*sigma_ij;

//         return force;
//     }

    __attribute__((noinline)) void local_calcs() {
        // iterate over the ocupied beads twice -- and do the inter device pairwise interactions
        while(s->local_slot_i) {
            if (tinselCanRecv()) {
                return;
            }
            int ci = get_next_slot(s->local_slot_i);
            if (s->local_slot_j == 0) {
                s->local_slot_j = s->bslot;
            }
            while(s->local_slot_j) {
                if (tinselCanRecv()) {
                    return;
                }
                int cj = get_next_slot(s->local_slot_j);
                if(ci != cj) {
                    // #ifndef TESTING
                        // s->force_slot[ci] = s->force_slot[ci] + force_update(&s->bead_slot[ci], &s->bead_slot[cj]);
                    // #else
                        // Vector3D<ptype> f = force_update(&s->bead_slot[ci], &s->bead_slot[cj]);

                        update_message1 m = { s->bead_slot[ci].pos,
                                              s->bead_slot[cj].pos,
                                              s->bead_slot[ci].velo,
                                              s->bead_slot[cj].velo,
                                              s->bead_slot[ci].id,
                                              s->bead_slot[cj].id,
                                              s->bead_slot[ci].pos.sq_dist(s->bead_slot[cj].pos),
                                              r_c,
                                              A[s->bead_slot[ci].type][s->bead_slot[cj].type],
                                              4.5, // drag_coefficient
                                              275.0, // sigma_ij
                                              0.1414 // square root of timestep (0.02)
                                            };
                        Vector3D<ptype> f = force_update1(&m);

                    // float r_ij_dist_sq = s->bead_slot[ci].pos.sq_dist(s->bead_slot[cj].pos);
                    // if (r_ij_dist_sq < (r_c*r_c)) {
                    //     update_message2 m = { newt_sqrt(r_ij_dist_sq),
                    //                           s->bead_slot[ci].pos - s->bead_slot[cj].pos,
                    //                           s->bead_slot[ci].velo - s->bead_slot[cj].velo,
                    //                           s->bead_slot[ci].id,
                    //                           s->bead_slot[cj].id,
                    //                           // (pairwise_rand(s->bead_slot[ci].id, s->bead_slot[cj].id) / (float)(DT10_RAND_MAX)) * 0.5,
                    //                           r_c,
                    //                           A[s->bead_slot[ci].type][s->bead_slot[cj].type],
                    //                           4.5, // drag_coefficient
                    //                           275.0, // sigma_ij
                    //                           0.1414 // square root of timestep (0.02)
                    //                         };

                    //     Vector3D<ptype> f = force_update2(&m);

                        Vector3D<int32_t> x = f.floatToFixed();
                        s->force_slot[ci] = s->force_slot[ci] + x;
                    // }
                    // #endif
                }
                s->local_slot_j = clear_slot(s->local_slot_j, cj);
            }
            s->local_slot_i = clear_slot(s->local_slot_i, ci);
        }
        s->sentslot = s->bslot;
    }

	// init handler -- called once by POLite at the start of execution
	inline void init() {
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
    #if !defined(TESTING) && !defined(STATS)
		s->emitcnt = emitperiod;
    #endif
		s->mode = UPDATE;
		if(get_num_beads(s->bslot) > 0)
		    *readyToSend = Pin(0);
        else
		    *readyToSend = No;
    #endif
	}

	// idle handler -- called once the system is idle with messages
	inline bool step() {
        // default case
        *readyToSend = No;
    #ifdef TIMER
        if (s->mode == START) {
            s->mode = UPDATE;
            if(get_num_beads(s->bslot) > 0)
                *readyToSend = Pin(0);
            else
                *readyToSend = No;
            s->dpd_startU = tinselCycleCountU();
            s->dpd_start  = tinselCycleCount();
            return true;
        } else
    #endif
        // we have just finished an update step
        if( s->mode == UPDATE ) {
        	s->mode = MIGRATION;
    	    s->timestep++;
        #if defined(TIMER) || defined(STATS)
            // Timed run has ended
            if (s->timestep >= TEST_LENGTH) {
            #ifndef STATS
                s->dpd_endU = tinselCycleCountU();
                s->dpd_end  = tinselCycleCount();
            #endif
                return false;
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
                s->force_slot[ci].set(0, 0, 0);
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
		            *readyToSend = Pin(0);
    	        }
    	        i = clear_slot(i, ci);
	        }
	        // we have finished updating -- now we want to migrate
	        return true;
        } // End of UPDATE mode block


	    // we have just finished a particle migration step
        if(s->mode == MIGRATION) {
        	// do we want to export?
        #if defined(TESTING)
            if (s->timestep >= TEST_LENGTH)
        #elif !defined(TIMER) && !defined(STATS)
        	if(s->emitcnt >= emitperiod)
        #endif
        #if !defined(TIMER) && !defined(STATS)
            {
    	        s->mode = EMIT;
	            if(s->bslot) {
    	            s->sentslot = s->bslot;
                    *readyToSend = HostPin;
    	        }
            #if !defined(TESTING) && !defined(STATS)
    	        s->emitcnt = 0;
            #endif
            } else {
            #if !defined(TESTING) && !defined(STATS)
    	        s->emitcnt++;
            #endif
    	        s->mode = UPDATE;
    	        if(get_num_beads(s->bslot) > 0){
    	            s->sentslot = s->bslot;
                    *readyToSend = Pin(0);
    	        }
	        }
            return true;
        #else
            s->mode = UPDATE;
            if(get_num_beads(s->bslot) > 0){
                s->sentslot = s->bslot;
                *readyToSend = Pin(0);
            }
            return true;
        #endif
	    }

        // we have just finished emitting the state to the host
    #if !defined(TIMER) && !defined(STATS)
	    if(s->mode == EMIT) {
        #if defined(TESTING) || defined(STATS)
            if (s->timestep >= TEST_LENGTH) {
                return false;
            }
        #endif
            // move into the update mode
	        s->mode = UPDATE;
	        s->sentslot = s->bslot;
	        if(get_num_beads(s->bslot) > 0){
                *readyToSend = Pin(0);
	        }
	        return true;
	    }
    #endif
        return false;
	}

	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
	    if(s->mode == UPDATE) {
	        uint32_t ci = get_next_slot(s->sentslot);
	        // send all of our beads to neighbours
	        msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].type = s->bead_slot[ci].type;
            msg->beads[0].id = s->bead_slot[ci].id;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());

	        s->sentslot = clear_slot(s->sentslot, ci);
	        if(s->sentslot != 0) {
                *readyToSend = Pin(0);
	        } else {
                s->local_slot_i = s->bslot;
                s->local_slot_j = s->bslot;
                *readyToSend = No;
                local_calcs();
	        }
	        return;
	    }

	    if(s->mode == MIGRATION) { // we are in the MIGRATION mode we want to send beads to our neighbours
	        // overload from with the dst filtering will happen on the recv side
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
	        if(s->migrateslot != 0) {
                *readyToSend = Pin(0);
	        } else {
                *readyToSend = No;
	        }
	        return;
	    }

	    // we are emitting our state to the host
    #if !defined(TIMER) && !defined(STATS)
	    if(s->mode==EMIT) {
	        // we are sending a host message
	        uint32_t ci = get_next_slot(s->sentslot);

	        msg->timestep = s->timestep;
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());
	        msg->beads[0].id = s->bead_slot[ci].id;
	        msg->beads[0].type = s->bead_slot[ci].type;


	        s->sentslot = clear_slot(s->sentslot, ci);
	        if(s->sentslot != 0) {
                *readyToSend = HostPin;
	        } else {
	            s->sentslot = s->bslot;
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
            msg->extra = tinselCycleCount();
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
        if(s->mode == UPDATE) {
	        // from the device locaton get the adjustments to the bead positions
            int x_rel = period_bound_adj(msg->from.x - s->loc.x);
            int y_rel = period_bound_adj(msg->from.y - s->loc.y);
            int z_rel = period_bound_adj(msg->from.z - s->loc.z);

	        // relative position for this particle to this device
	        msg->beads[0].pos.x(msg->beads[0].pos.x() + ptype(x_rel)*s->unit_size);
	        msg->beads[0].pos.y(msg->beads[0].pos.y() + ptype(y_rel)*s->unit_size);
	        msg->beads[0].pos.z(msg->beads[0].pos.z() + ptype(z_rel)*s->unit_size);

            // loop through the occupied bead slots -- update force
	        uint32_t i = s->bslot;
	        while(i) {
                int ci = get_next_slot(i);
                // #ifndef TESTING
                    // s->force_slot[ci] = s->force_slot[ci] + force_update(&s->bead_slot[ci], &msg->beads[0]);
                // #else
                    // Vector3D<ptype> f = force_update(&s->bead_slot[ci], &msg->beads[0]);

                    update_message1 m = { s->bead_slot[ci].pos,
                                          msg->beads[0].pos,
                                          s->bead_slot[ci].velo,
                                          msg->beads[0].velo,
                                          s->bead_slot[ci].id,
                                          msg->beads[0].id,
                                          s->bead_slot[ci].pos.sq_dist(msg->beads[0].pos),
                                          1.0, // r_c
                                          A[s->bead_slot[ci].type][msg->beads[0].type],
                                          4.5, // drag_coefficient
                                          275.0, // sigma_ij
                                          0.1414 // square root of timestep (0.02)
                                        };
                    Vector3D<ptype> f = force_update1(&m);

                //    float r_ij_dist_sq = s->bead_slot[ci].pos.sq_dist(msg->beads[0].pos);
                // if (r_ij_dist_sq < (r_c*r_c)) {
                //     update_message2 m = { newt_sqrt(r_ij_dist_sq),
                //                           s->bead_slot[ci].pos - msg->beads[0].pos,
                //                           s->bead_slot[ci].velo - msg->beads[0].velo,
                //                           s->bead_slot[ci].id,
                //                           msg->beads[0].id,
                //                           // (pairwise_rand(s->bead_slot[ci].id, msg->beads[0].id) / (float)(DT10_RAND_MAX)) * 0.5,
                //                           r_c,
                //                           A[s->bead_slot[ci].type][msg->beads[0].type],
                //                           4.5, // drag_coefficient
                //                           275.0, // sigma_ij
                //                           0.1414 // square root of timestep (0.02)
                //                         };

                //     Vector3D<ptype> f = force_update2(&m);

                    Vector3D<int32_t> x = f.floatToFixed();
                    s->force_slot[ci] = s->force_slot[ci] + x;
                // }
                // #endif
	            i = clear_slot(i, ci);
	        }
            if (s->sentslot == 0) {
                local_calcs();
            }
	    } else if (s->mode == MIGRATION) {
            // we are in the MIGRATION mode beads we receive here _may_ be added to our state
	        // when we receive a message it _may_ contain a bead that we need to add to our state
	        // it depends on whether the from address matches our own
	        if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
	            // looks like we are getting a new addition to our family
	            uint32_t ci = get_next_free_slot(s->bslot); // I hope we have space...
                // if (ci == 0xFFFFFFFF) {
                //     s->lost_beads++;
                // } else {
                    s->bslot = set_slot(s->bslot, ci);
    	            s->sentslot = s->bslot;

    	            // welcome the new little bead
    	            s->bead_slot[ci].type = msg->beads[0].type;
    	            s->bead_slot[ci].id = msg->beads[0].id;
    	            s->bead_slot[ci].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
    	            s->bead_slot[ci].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                // }
	        }
        }
	}

	// finish -- sends a message to the host on termination
	inline bool finish(volatile DPDMessage* msg) {
    #if defined(TESTING) || defined(STATS)
        msg->type = 0xAA;
    #endif

    #ifdef TIMER
        msg->type = 0xAA;
        msg->from.x = s->loc.x;
        msg->from.y = s->loc.y;
        msg->from.z = s->loc.z;
        msg->timestep = s->dpd_startU;
        msg->extra = s->dpd_start;
        msg->beads[0].id = s->dpd_endU;
        msg->beads[0].type = s->dpd_end;
    #endif
	    return true;
    }

};

#endif /* _DPD_H */
