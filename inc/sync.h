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

#include "dpd.hpp"
#include "Vector3D.hpp"
#include "BeadMap.hpp"
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

#ifdef VISUALISE
const uint32_t emitperiod = 1;
#endif

// ---------------------------------------------------------------------------------------

typedef uint8_t bead_class_t; // the type of the bead, we are not expecting too many
typedef uint32_t bead_id_t; // the ID for the bead

// Format of message
struct DPDMessage {
    uint8_t type;
    uint32_t timestep; // the timestep this message is from
    cell_t from; // the unit that this message is from
    bead_t beads[1]; // the beads payload from this unit
}; // 50 Bytes

// the state of the DPD Device
struct DPDState {
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t N;
    cell_t loc; // the location of this cube
    uint32_t bslot; // a bitmap of which bead slot is occupied
    uint32_t sentslot; // a bitmap of which bead slot has not been sent from yet
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
    Vector3D<int32_t> force_slot[MAX_BEADS]; // at most 5 beads -- force for each bead
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint32_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in 0 = update; 1 = migration
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    uint32_t lost_beads; // Beads lost due to the cell having a full bead_slot
    uint32_t max_timestep; // Maximum timestep for this run

#ifdef SMALL_DT_EARLY
    ptype dt;
    ptype inv_sqrt_dt;
#endif

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

    // dt10's random number generator


	// init handler -- called once by POLite at the start of execution
	inline void init() {
    #ifdef MESSAGE_COUNTER
        s->message_counter = 0;
    #endif
		// s->rngstate = 1234; // start with a seed
		s->grand = p_rand(&s->rngstate);
    #ifdef VISUALISE
		s->emitcnt = emitperiod;
    #endif
		// s->mode = UPDATE;
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
        #ifdef SMALL_DT_EARLY
            if (s->timestep == 1000) {
                s->dt = normal_dt;
                s->inv_sqrt_dt = normal_inv_sqrt_dt;
            }
        #endif
        #if defined(TIMER) || defined(STATS) || defined(MESSAGE_COUNTER)
            // Timed run has ended
            if (s->timestep >= s->max_timestep) {
                return false;
            }
        #endif
    	    s->grand = p_rand(&s->rngstate); // advance the random number
    	    uint32_t i = s->bslot;
    	    while(i){
                uint8_t ci = get_next_slot(i);
            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], &s->old_velo[ci], s->dt);
            #elif defined(SMALL_DT_EARLY)
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], s->dt);
            #elif defined(BETTER_VERLET)
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], &s->old_velo[ci], dt);
            #else
                velocity_Verlet(&s->bead_slot[ci], &s->force_slot[ci], dt);
            #endif

            #if defined(SMALL_DT_EARLY) && defined(BETTER_VERLET)
                if (migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->N, &s->migrateslot, &s->migrate_loc[ci], s->dt, &s->old_velo[ci])) {
            #elif defined(SMALL_DT_EARLY)
                if (migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->N, &s->migrateslot, &s->migrate_loc[ci], s->dt)) {
            #elif defined(BETTER_VERLET)
                if (migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->N, &s->migrateslot, &s->migrate_loc[ci], dt, &s->old_velo[ci])) {
            #else
                if (migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->N, &s->migrateslot, &s->migrate_loc[ci], dt)) {
            #endif
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
                s->emitcnt = 1;
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
            if (s->timestep >= s->max_timestep) {
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
        if (s->timestep >= s->max_timestep) {
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
          #ifdef SMALL_DT_EARLY
            local_calcs(ci, s->inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
          #else
            local_calcs(ci, inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
          #endif
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
              #ifdef SMALL_DT_EARLY
                local_calcs(s->inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
              #else
                local_calcs(inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
              #endif
            #endif
	        }
        #else
            if (s->sentslot == 0) {
                *readyToSend = No; // No more beads to send
                msg->type = 0xBC; // 0xBC represents this is my last bead.
            #if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
              #ifdef SMALL_DT_EARLY
                local_calcs(s->inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
              #else
                local_calcs(inv_sqrt_dt, s->bslot, s->bead_slot, s->grand, s->force_slot);
              #endif
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
          #ifndef TESTING
            msg->beads[0].velo.set(s->bead_slot[ci].velo.x(), s->bead_slot[ci].velo.y(), s->bead_slot[ci].velo.z());
          #endif
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
                // Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b);
                #ifdef SMALL_DT_EARLY
                  Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b, s->grand, s->inv_sqrt_dt);
                #else
                  Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b, s->grand, inv_sqrt_dt);
                #endif
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

                    // Welcome the new little bead
                #ifndef BETTER_VERLET
                    s->bead_slot[ci].type = msg->beads[0].type;
                    s->bead_slot[ci].id = msg->beads[0].id;
                    s->bead_slot[ci].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                    s->bead_slot[ci].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                    s->force_slot[ci].set(0.0, 0.0, 0.0);
                #else
                    s->bead_slot[ci].type = msg->beads[0].type;
                    s->bead_slot[ci].id = msg->beads[0].id;
                    s->bead_slot[ci].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                    s->bead_slot[ci].acc.set(msg->beads[0].acc.x(), msg->beads[0].acc.y(), msg->beads[0].acc.z());
                    s->force_slot[ci].set(0.0, 0.0, 0.0);

                    // Store old velocity
                    s->old_velo[ci].set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                    // Update velocity
                #ifndef SMALL_DT_EARLY
                    update_velocity(&s->bead_slot[ci], &s->old_velo[ci], dt);
                #else
                    update_velocity(&s->bead_slot[ci], &s->old_velo[ci], s->dt);
                #endif
                #endif
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