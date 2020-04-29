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
#define MIGRATION 1

#if defined(VISUALISE) || defined(TESTING)
#define EMIT 2
#endif

typedef float ptype;

#ifdef MESSAGE_MANAGEMENT
  #ifndef SEND_TO_SELF
    const uint8_t NEIGHBOURS = 26;
  #else
    const uint8_t NEIGHBOURS = 27;
  #endif
#endif


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
const ptype bond_kappa=100; // Bond interaction is very strong
const ptype bond_r0=0.5; // Distance of 0.5 to avoid escaping

 inline bool are_beads_bonded(uint32_t a, uint32_t b)
{
    return (a&b&0x80000000ul) && (((a-b)==1) || ((b-a)==1));
}

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
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
    uint32_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    unit_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in 0 = update; 1 = migration
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    uint32_t lost_beads; // Beads lost due to the cell having a full bead_slot
    uint32_t max_time; // Maximum timestep for this run

#ifdef MESSAGE_MANAGEMENT
    int8_t msgs_to_recv; // Number of messages expected from neighbours. Will only send when all neighbours have sent at least one message
    uint8_t nbs_complete; // Neighbours which are not expected to send any more. Works in tandem with the above
#endif

#ifdef MESSAGE_COUNTER
    uint32_t message_counter;
#endif
};

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

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

// =============== MAY BE NEEDED FOR DEBUG, NOT NEEDED FOR RUNS ===============
    // get the number of beads occupying a slot
    // inline uint32_t get_num_beads(uint32_t slotlist){
    //     uint32_t cnt = 0;
    //     uint32_t mask = 0x1;
    //     for(int i=0; i<MAX_BEADS; i++){
    //             if(slotlist & mask) {
    //                   cnt++;
    //             }
    //             mask = mask << 1;
    //     }
    //     return cnt; // error there are no free slots!
    // }
    // --------------------------------------------------------
// ============================================================================

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

#ifndef ACCELERATE
    // calculate a new force acting between two particles
#if defined(TESTING) || defined(STATS)
    __attribute__((noinline)) Vector3D<ptype> force_update(bead_t *a, bead_t *b){
#else
    Vector3D<ptype> force_update(bead_t *a, bead_t *b){
#endif

        ptype r_ij_dist_sq = a->pos.sq_dist(b->pos);

        Vector3D<ptype> force(0.0,0.0,0.0); // accumulate the force here

        if (r_ij_dist_sq > sq_r_c) {
            return force;
        }

        ptype r_ij_dist = newt_sqrt(r_ij_dist_sq); // Only square root for distance once it's known these beads interact

        ptype a_ij = A[a->type][b->type];
        Vector3D<ptype> r_ij = a->pos - b->pos;
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
        ptype r_t((pairwise_rand(a->id, b->id) / (float)(DT10_RAND_MAX/2)));
        ptype r = (r_t - ptype(1.0)) * 0.5;
        ptype w_r = (ptype(1.0) - r_ij_dist);

        // random force
        ptype ran = sqrt_dt*r*w_r*sigma_ij;
        force = force - ((r_ij / r_ij_dist) * ran);

        if(are_beads_bonded(a->id, b->id)) {
            force = force - (r_ij / r_ij_dist) * bond_kappa * (r_ij_dist-bond_r0);
        }

        return force;
    }
#endif

#if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
    __attribute__((noinline)) void local_calcs() {
        // iterate over the ocupied beads twice -- and do the inter device pairwise interactions
        uint32_t i = s->bslot;
        while(i) {
            int ci = get_next_slot(i);
            uint32_t j = s->bslot;
            while(j) {
                int cj = get_next_slot(j);
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
            i = clear_slot(i, ci);
        }
    }
#elif defined(ONE_BY_ONE)
    void local_calcs(uint32_t ci) {
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
#endif

	// init handler -- called once by POLite at the start of execution
	inline void init() {
    #if defined(TESTING) || defined(STATS)
        s->max_time = 1000;
    #elif defined(MESSAGE_COUNTER)
        s->message_counter = 0;
    #endif
		s->rngstate = 1234; // start with a seed
		s->grand = rand();
    #ifdef VISUALISE
		s->emitcnt = emitperiod;
    #endif
		s->mode = UPDATE;
        s->sentslot = s->bslot;
    #ifndef MESSAGE_MANAGEMENT
		if (s->sentslot) {
		    *readyToSend = Pin(0);
        } else {
		    *readyToSend = No;
        }
    #else
        *readyToSend = Pin(0);
        s->nbs_complete = 0;
        s->msgs_to_recv = 0;
    #endif
	}

	// idle handler -- called once the system is idle with messages
	inline bool step() {
        // default case
        *readyToSend = No;

        // we have just finished an update step
        if( s->mode == UPDATE ) {
        	s->mode = MIGRATION;
    	    s->timestep++;
        #if defined(TIMER) || defined(STATS) || defined(MESSAGE_COUNTER)
            // Timed run has ended
            if (s->timestep >= s->max_time) {
                return false;
            }
        #endif
    	    s->grand = rand(); // advance the random number
    	    uint32_t i = s->bslot;
    	    while(i){
                int ci = get_next_slot(i);

                // ------ velocity verlet ------
                Vector3D<ptype> force = s->force_slot[ci].fixedToFloat();
                Vector3D<ptype> acceleration = force / p_mass;
                Vector3D<ptype> delta_v = acceleration * dt;
                // update velocity
                s->bead_slot[ci].velo = s->bead_slot[ci].velo + delta_v;
                // update position
                s->bead_slot[ci].pos = s->bead_slot[ci].pos + s->bead_slot[ci].velo*dt + acceleration*ptype(0.5)*dt*dt;

                // ----- clear the forces ---------------
                s->force_slot[ci].set(0, 0, 0);

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
        #ifdef VISUALISE
            if(s->emitcnt >= emitperiod) {
                s->mode = EMIT;
                if(s->bslot) {
                    s->sentslot = s->bslot;
                    *readyToSend = HostPin;
                }
                s->emitcnt = 0;
            } else {
                s->emitcnt++;
                s->mode = UPDATE;
                s->sentslot = s->bslot;
            #ifndef MESSAGE_MANAGEMENT
                if(s->sentslot){
                    *readyToSend = Pin(0);
                }
            #else
                *readyToSend = Pin(0);
                s->nbs_complete = 0;
                s->msgs_to_recv = 0;
            #endif
            }
            return true;
        #elif defined(TESTING)
            if (s->timestep >= s->max_time) {
                s->mode = EMIT;
                if(s->bslot) {
                    s->sentslot = s->bslot;
                    *readyToSend = HostPin;
                }
            } else {
                s->mode = UPDATE;
                s->sentslot = s->bslot;
            #ifndef MESSAGE_MANAGEMENT
                if(s->sentslot){
                    *readyToSend = Pin(0);
                }
            #else
                *readyToSend = Pin(0);
                s->nbs_complete = 0;
                s->msgs_to_recv = 0;
            #endif
            }
            return true;
        #else
            s->mode = UPDATE;
            s->sentslot = s->bslot;
        #ifndef MESSAGE_MANAGEMENT
            if(s->sentslot){
                *readyToSend = Pin(0);
            }
        #else
            *readyToSend = Pin(0);
            s->nbs_complete = 0;
            s->msgs_to_recv = 0;
        #endif
            return true;
        #endif

	    }

        // we have just finished emitting the state to the host
    #ifdef VISUALISE
        if (s->mode == EMIT) {
            s->mode = UPDATE;
            s->sentslot = s->bslot;
        #ifndef MESSAGE_MANAGEMENT
            if(s->sentslot){
                *readyToSend = Pin(0);
            }
        #else
            *readyToSend = Pin(0);
            s->nbs_complete = 0;
            s->msgs_to_recv = 0;
        #endif

            return true;
        }
    #elif defined(TESTING) || defined(STATS)
        if (s->timestep >= s->max_time) {
            return false;
        }
    #endif

        return false;
	}

	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
    #ifdef MESSAGE_COUNTER
        s->message_counter++;
    #endif
	    if(s->mode == UPDATE) {
        #ifdef MESSAGE_MANAGEMENT
            s->msgs_to_recv += NEIGHBOURS - s->nbs_complete; // Only expect messages from neighbours who have beads
            if (s->sentslot == 0) {
                msg->type = 0xBB; // 0xBB represents I have no beads to send
                *readyToSend = No;
                return;
            }
            msg->type = 0x00; // 0 represents a standard update message
        #endif

	        uint32_t ci = get_next_slot(s->sentslot);

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
        #ifndef MESSAGE_MANAGEMENT
	        if(s->sentslot) {
                *readyToSend = Pin(0);
	        } else {
                *readyToSend = No;
            #if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
                local_calcs();
            #endif
	        }
        #else
            if (s->sentslot == 0) {
                *readyToSend = No; // No more beads to send
                msg->type = 0xBC; // 0xBC represents this is my last bead.
            #if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
                local_calcs();
            #endif
            } else if (s->msgs_to_recv <= 0 || s->nbs_complete == NEIGHBOURS) {
                *readyToSend = Pin(0); // We have beads to send, and we've received a round of messages from all neighbours
            } else {
                *readyToSend = No; // We need to wait for a full round of messages from neighbours
            }
        #endif
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

	        if(s->migrateslot) {
                *readyToSend = Pin(0);
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

	        msg->timestep = s->timestep;
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].pos.set(s->bead_slot[ci].pos.x(), s->bead_slot[ci].pos.y(), s->bead_slot[ci].pos.z());
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());
	        msg->beads[0].id = s->bead_slot[ci].id;
	        msg->beads[0].type = s->bead_slot[ci].type;

	        s->sentslot = clear_slot(s->sentslot, ci);
	        if(s->sentslot) {
                *readyToSend = HostPin;
	        } else {
	            *readyToSend = No;
	        }
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
        if(s->mode == UPDATE) {

        #ifdef MESSAGE_MANAGEMENT
            s->msgs_to_recv--; // We expect a message from every neighbour
            if (msg->type == 0xBB) {
                s->nbs_complete++; // The sender had no beads and wont send any more
                set_rts();
                return; // We're done with this message - No beads from this neighbour
            } else if (msg->type == 0xBC) {
                s->nbs_complete++; // This is the senders last message, but the bead in it is useful
            }
            set_rts();
        #endif

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
            #ifdef SEND_TO_SELF
                if(s->bead_slot[ci].id != b.id) {
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
                }
            #else
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
            #endif

	            i = clear_slot(i, ci);
	        }
	    } else if (s->mode == MIGRATION) {
            // we are in the MIGRATION mode beads we receive here _may_ be added to our state
	        // when we receive a message it _may_ contain a bead that we need to add to our state
	        // it depends on whether the from address matches our own
	        if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
	            // looks like we are getting a new addition to our family
	            uint32_t ci = get_next_free_slot(s->bslot); // I hope we have space...
                if (ci == 0xFFFFFFFF) {
                    s->lost_beads++;
                } else {
                    s->bslot = set_slot(s->bslot, ci);
    	            s->sentslot = s->bslot;

    	            // welcome the new little bead
    	            s->bead_slot[ci].type = msg->beads[0].type;
    	            s->bead_slot[ci].id = msg->beads[0].id;
    	            s->bead_slot[ci].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
    	            s->bead_slot[ci].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                }
	        }
        }
	}

	// finish -- sends a message to the host on termination
	inline bool finish(volatile DPDMessage* msg) {
        msg->type = 0xAA;
        msg->timestep = s->timestep;
    #ifdef MESSAGE_COUNTER
        msg->from.x = s->loc.x;
        msg->from.y = s->loc.y;
        msg->from.z = s->loc.z;
        msg->timestep = s->message_counter;
    #endif
	    return true;
    }

};

#endif /* _DPD_H */
