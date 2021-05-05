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

// ---------------------------------------------------------------------------------------

typedef uint8_t bead_class_t; // the type of the bead, we are not expecting too many
typedef uint32_t bead_id_t; // the ID for the bead

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

	// init handler -- called once by POLite at the start of execution
	inline void init() {
    #ifdef MESSAGE_COUNTER
        s->message_counter = 0;
    #endif
		// s->rngstate = 1234; // start with a seed
		s->grand = p_rand(&s->rngstate);
        s->grand = p_rand(&s->rngstate);
    #ifdef VISUALISE
		s->emitcnt = 1;
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
    	    uint16_t i = s->bslot;
    	    while(i){
                uint8_t ci = get_next_slot(i);

                velocity_Verlet(ci, s);

                if (migration(ci, s)) {
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
        if (s->error) {
            msg->type = s->error;
            msg->timestep = s->timestep;
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].id = get_num_beads(s->bslot);
            return;
        }
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

	        uint16_t ci = get_next_slot(s->sentslot);
            // Clear this from the slot
            s->sentslot = clear_slot(s->sentslot, ci);

        #ifdef ONE_BY_ONE
              #ifdef REDUCE_LOCAL_CALCS
                // Pass in a beadmap containing only the beads which have yet to be sent.
                // They will have the resulting force subtracted from their accumulated force
                // This should reduce the number of calls to force_update for local bead interactions
               #ifndef SINGLE_FORCE_LOOP
                local_calcs(ci, s->sentslot, s);
               #else
                calc_bead_force_on_beads(&s->bead_slot[ci], s->sentslot, s, ci);
               #endif
              #else
               #ifndef SINGLE_FORCE_LOOP
                local_calcs(ci, s->bslot, s);
               #else
                calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, s);
               #endif
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

        #ifndef MESSAGE_MANAGEMENT
	        if(s->sentslot) {
                *readyToSend = Pin(0);
	        } else {
                *readyToSend = No;
            #if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
                #ifdef SMALL_DT_EARLY
                  #ifdef REDUCE_LOCAL_CALCS
                    // Pass in a beadmap containing only the beads which have yet to be sent.
                    // They will have the resulting force subtracted from their accumulated force
                    // This should reduce the number of calls to force_update for local bead interactions
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, s->inv_sqrt_dt, s->sentslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->sentslot, s, ci);
                   #endif
                  #else
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, s->inv_sqrt_dt, s->bslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, s->inv_sqrt_dt, s);
                   #endif
                  #endif
                #else
                  #ifdef REDUCE_LOCAL_CALCS
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, inv_sqrt_dt, s->sentslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->sentslot, s, ci);
                   #endif
                  #else
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(inv_sqrt_dt, s->bslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, inv_sqrt_dt, s);
                   #endif
                  #endif
                #endif
            #endif
	        }
        #else
            if (s->sentslot == 0) {
                *readyToSend = No; // No more beads to send
                msg->type = 0xBC; // 0xBC represents this is my last bead.
            #if !defined(ONE_BY_ONE) && !defined(SEND_TO_SELF)
                #ifdef SMALL_DT_EARLY
                  #ifdef REDUCE_LOCAL_CALCS
                    // Pass in a beadmap containing only the beads which have yet to be sent.
                    // They will have the resulting force subtracted from their accumulated force
                    // This should reduce the number of calls to force_update for local bead interactions
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, s->inv_sqrt_dt, s->sentslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->sentslot, s->inv_sqrt_dt, s, ci);
                   #endif
                  #else
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, s->inv_sqrt_dt, s->bslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, s->inv_sqrt_dt, s);
                   #endif
                  #endif
                #else
                  #ifdef REDUCE_LOCAL_CALCS
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, inv_sqrt_dt, s->sentslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->sentslot, inv_sqrt_dt, s, ci);
                   #endif
                  #else
                   #ifndef SINGLE_FORCE_LOOP
                    local_calcs(ci, inv_sqrt_dt, s->bslot, s);
                   #else
                    calc_bead_force_on_beads(&s->bead_slot[ci], s->bslot, inv_sqrt_dt, s);
                   #endif
                  #endif
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
            #ifdef TESTING
                msg->type = 0xAA;
            #endif
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

            bead_t b = get_relative_bead(&msg->beads[0], &s->loc, &msg->from);

        #ifndef SINGLE_FORCE_LOOP
            // loop through the occupied bead slots -- update force
	        uint16_t i = s->bslot;
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
                  Vector3D<ptype> f = force_update(&s->bead_slot[ci], &b, s);
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
              #ifdef FLOAT_ONLY
                s->force_slot[ci] = s->force_slot[ci] + f;
              #else
                Vector3D<int32_t> x = f.floatToFixed();
                s->force_slot[ci] = s->force_slot[ci] + x;
              #endif
            #endif

	            i = clear_slot(i, ci);
	        }
        #else
            calc_bead_force_on_beads(&b, s->bslot, s);
        #endif
	    } else if (s->mode == MIGRATION) {
            // we are in the MIGRATION mode beads we receive here _may_ be added to our state
	        // when we receive a message it _may_ contain a bead that we need to add to our state
	        // it depends on whether the from address matches our own
	        if( (msg->from.x == s->loc.x) && (msg->from.y == s->loc.y) && (msg->from.z == s->loc.z) ) {
	            // looks like we are getting a new addition to our family
	            uint32_t ci = get_next_free_slot(s->bslot); // I hope we have space...
                if (ci == 0xFF) {
                    s->error = 0xE0;
                    *readyToSend = HostPin;
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
                    update_velocity(ci, s);
                #else
                    update_velocity(ci, s);
                #endif
                #endif
                }
	        }
        }
	}

	// finish -- sends a message to the host on termination
	inline bool finish(volatile DPDMessage* msg) {
        msg->type = 0xAB;
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
