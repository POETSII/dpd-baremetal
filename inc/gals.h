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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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
const uint32_t emitperiod = 1000;
#endif

// Format of message
struct DPDMessage {
    uint8_t mode;
    uint8_t type;
    uint32_t timestep; // The timestep this message is from
    uint8_t total_beads; // Used for sending cycle counts
    cell_t from; // The cell that this message is from
    bead_t beads[1]; // The beads payload from this unit
}; // 48 bytes - 60 bytes with BETTER_VERLET

// the state of the DPD Device
struct DPDState {
    float unit_size; // the size of this spatial unit in one dimension
    uint8_t cells_per_dimension;
    cell_t loc; // the location of this cube
    uint16_t bslot; // a bitmap of which bead slot is occupied
    uint16_t sentslot; // a bitmap of which bead slot has not been sent from yet
    uint16_t newBeadMap;
    bead_t bead_slot[MAX_BEADS]; // at most we have five beads per device
#ifdef FLOAT_ONLY
    Vector3D<float> force_slot[MAX_BEADS];
#else
    Vector3D<int32_t> force_slot[MAX_BEADS]; // force for each bead
#endif
#ifdef BETTER_VERLET
    Vector3D<ptype> old_velo[MAX_BEADS]; // Store old velocites for verlet
#endif
    uint16_t migrateslot; // a bitmask of which bead slot is being migrated in the next phase
    cell_t migrate_loc[MAX_BEADS]; // slots containing the destinations of where we want to send a bead to
    uint8_t mode; // the mode that this device is in.
#ifdef VISUALISE
    uint32_t emitcnt; // a counter to kept track of updates between emitting the state
#endif
    uint32_t timestep; // the current timestep that we are on
    uint32_t grand; // the global random number at this timestep
    uint64_t rngstate; // the state of the random number generator

    // uint32_t lost_beads;
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
    uint8_t error; // Error code to be returned to the host
};

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

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
        s->grand = p_rand(&s->rngstate); // advance the random number
        // ------ velocity verlet ------
        uint16_t i = s->bslot;
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
            migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->cells_per_dimension, &s->migrateslot, &s->migrate_loc[ci], s->dt, &s->old_velo[ci]);
        #elif defined(SMALL_DT_EARLY)
            migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->cells_per_dimension, &s->migrateslot, &s->migrate_loc[ci], s->dt);
        #elif defined(BETTER_VERLET)
            migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->cells_per_dimension, &s->migrateslot, &s->migrate_loc[ci], dt, &s->old_velo[ci]);
        #else
            migration(ci, &s->bead_slot[ci], s->unit_size, s->loc, s->cells_per_dimension, &s->migrateslot, &s->migrate_loc[ci], dt);
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

    __attribute__((noinline)) bool emit_complete() {
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

	// init handler -- called once by POLite at the start of execution
	inline void init() {
		s->rngstate = 1234; // start with a seed
		s->grand = p_rand(&s->rngstate);
        s->grand = p_rand(&s->rngstate);
		// s->mode = UPDATE;
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
        msg->type = 0x00;
        msg->timestep = s->timestep;

        // If there has been an error, send it to the host and then halt
        if (s->error) {
            msg->type = s->error;
            *readyToSend = No;
            return;
        }

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
            msg->type = 0x00;
	        uint8_t ci = get_next_slot(s->sentslot);
            // Already done at start of send()
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
	        if(s->sentslot != 0) {
                *readyToSend = HostPin;
	        } else {
            #ifndef TESTING
	            s->sentslot = s->bslot;
                s->mode = EMIT_COMPLETE;
	            *readyToSend = Pin(0);
            #else
                // Bypasses the need for another mostly empty message
                // Works for testing as the only time we emit is at the end
                msg->type = 0xAA; // Signifies to the host that the testing is complete
                *readyToSend = No;
            #endif
	        }
            return;
	    }
    #endif

        if (s->mode==EMIT_COMPLETE) {
            // Tell neighbours I have finished emitting.
            msg->mode = EMIT_COMPLETE;
            s->emit_complete_sent = 2;
            // s->emit_completes_received++;
            if (!emit_complete()) {
                *readyToSend = No;
            }
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

	// // used to help adjust the relative positions for the periodic boundary
 //    __attribute__((noinline)) int8_t period_bound_adj(int8_t dim) {
 //        if(dim > 1) {
 //            return -1;
 //        } else if (dim < -1) {
 //            return 1;
 //        } else {
 //            return dim;
 //        }
 //    }

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
                    uint16_t i = s->bslot;
                    while(i) {
                        uint8_t ci = get_next_slot(i);
                    #ifndef ONE_BY_ONE
                        if(s->bead_slot[ci].id != b.id) {
                    #endif
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
                        #ifdef FLOAT_ONLY
                            s->force_slot[ci] = s->force_slot[ci] + f;
                        #else
                            Vector3D<int32_t> x = f.floatToFixed();
                            s->force_slot[ci] = s->force_slot[ci] + x;
                        #endif
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
                        if (ni == 0xFF) {
                            s->error = 0xE0;
                            *readyToSend = HostPin;
                            return;
                        } else {
                            s->newBeadMap = set_slot(s->newBeadMap, ni);

                        #ifndef BETTER_VERLET
                            s->bead_slot[ni].type = msg->beads[0].type;
                            s->bead_slot[ni].id = msg->beads[0].id;
                            s->bead_slot[ni].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                            s->bead_slot[ni].velo.set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                            // Force slot should be cleared anyway
                        #else
                            // Welcome the new little bead
                            s->bead_slot[ni].type = msg->beads[0].type;
                            s->bead_slot[ni].id = msg->beads[0].id;
                            s->bead_slot[ni].pos.set(msg->beads[0].pos.x(), msg->beads[0].pos.y(), msg->beads[0].pos.z());
                            s->bead_slot[ni].acc.set(msg->beads[0].acc.x(), msg->beads[0].acc.y(), msg->beads[0].acc.z());
                            // Force slot should be cleared anyway

                            // Store old velocity
                            s->old_velo[ni].set(msg->beads[0].velo.x(), msg->beads[0].velo.y(), msg->beads[0].velo.z());
                            // Update velocity
                          #ifndef SMALL_DT_EARLY
                            update_velocity(&s->bead_slot[ni], &s->old_velo[ni], dt);
                          #else
                            update_velocity(&s->bead_slot[ni], &s->old_velo[ni], s->dt);
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
