// This header file is shared by the tinsel program and the host PC program
// It is used to define various DPD particles

#include <stdint.h>
#include <POLite.h>

#include "Vector3D.hpp"

#ifndef _DPD_H_
#define _DPD_H_

#define BEADS_PER_UNIT 2 
#define UNIT_SPACE 1.0 // a cube 1.0 x 1.0 x 1.0
#define PADDING 0 

typedef float ptype;

// ------------------------- SIMULATION PARAMETERS --------------------------------------

const float problem_size = 10.0; // total size of the sim universe in one dimension
const unsigned N = 10; // the size of the sim universe in each dimension

const ptype r_c(1.0);

// interaction matrix
const ptype A[3][3] = {  {ptype(25.0), ptype(75.0), ptype(35.0)},
                         {ptype(75.0), ptype(25.0), ptype(50.0)},
                         {ptype(35.0), ptype(50.0), ptype(25.0)}}; // interaction matrix

const ptype dt = 0.02; // the timestep
const ptype p_mass = 1.0; // the mass of all beads (not currently configurable per bead)

// ---------------------------------------------------------------------------------------


typedef uint16_t bead_class_t; // the type of the bead, we are not expecting too many 
typedef uint16_t bead_id_t; // the ID for the bead

// defines a bead type
typedef struct _bead_t {
    bead_class_t type;
    bead_id_t id;
    Vector3D<ptype> pos;
    Vector3D<ptype> velo;
} bead_t; // 28 bytes 

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
  float debug;
  unit_t from; // the unit that this message is from 
  bead_t beads[1]; // the beads payload from this unit 
}; 

// the state of the DPD Device
struct DPDState{
   float debug_cnt;
   float unit_size; // the size of this spatial unit in one dimension
   unit_t loc; // the location of this cube
   uint8_t bslot; // a bitmap of which bead slot is occupied
   uint8_t sentslot; // a bitmap of which bead slot has not been sent from yet 
   uint8_t num_beads; // the number of beads in this device
   bead_t bead_slot[5]; // at most we have five beads per device
   Vector3D<ptype> force_slot[5]; // at most 5 beads -- force for each bead
   uint8_t migrate; // a bitmask of which bead slot is being migrated in the next phase 

   // send tracking
   uint8_t sentcnt; // a counter used to track how many beads have been sent
   uint8_t sendmode; // keeps track of what mode this device is in sendmode = 0 force_update; sendmode = 1 migration
}; 

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

	// ----------------- bead slots ---------------------------
	// helper functions for managing bead slots
        inline uint8_t clear_slot(uint8_t slotlist, uint8_t pos){  return slotlist & ~(1 << pos);  }
        inline uint8_t set_slot(uint8_t slotlist, uint8_t pos){ return slotlist | (1 << pos); }
        inline bool is_slot_set(uint8_t slotlist, uint8_t pos){ return slotlist & (1 << pos); }
        
        inline uint8_t get_next_slot(uint8_t slotlist){
            uint8_t mask = 0x1;
            for(int i=0; i<8; i++) {
                if(slotlist & mask){
                        return i;
                }
                mask = mask << 1; // shift to the next pos
            }
            return 0xF; // we are empty
        }
        
        inline uint8_t get_next_free_slot(uint8_t slotlist){
            uint8_t mask = 0x1;
            for(int i=0; i<8; i++){
                    if(!(slotlist & mask)) {
                           return i;
                    }
                    mask = mask << 1;
            }
            return 0xF; // error there are no free slots!
        }

	// get the number of beads occupying a slot
        inline uint8_t get_num_beads(uint8_t slotlist){
            uint8_t cnt = 0;
            uint8_t mask = 0x1;
            for(int i=0; i<8; i++){
                    if(slotlist & mask) {
                          cnt++;
                    }
                    mask = mask << 1;
            }
            return cnt; // error there are no free slots!
        }
	// --------------------------------------------------------



        // calculate a new force acting between two particles
        Vector3D<ptype> force_update(bead_t *a, bead_t *b){
            ptype a_ij = A[a->id][b->id]; 
            Vector3D<ptype> r_i = a->pos;
            Vector3D<ptype> r_j = b->pos;
            ptype r_ij_dist = r_i.dist(r_j);
            Vector3D<ptype> r_ij = r_i - r_j;
        
            // Equation 8.5 in the dl_meso manual
            Vector3D<ptype> force = (r_ij/r_ij_dist) * (a_ij * (ptype(1.0) - (r_ij_dist/r_c)));
        
            return force;
        }

	// init handler -- called once by POLite at the start of execution
	inline void init() {
		s->sentslot = s->bslot;
		if(get_num_beads(s->bslot) > 0)
		    *readyToSend = Pin(0);
	        else
		    *readyToSend = No;
	}
	
	// idle handler -- called once the system is idle with messages
	inline void idle() {

            // iterate over all beads in this device and perform velocity verlet
	    uint8_t i = s->bslot;
	    while(i){
               int ci = get_next_slot(i);

	       // ------ velocity verlet ------
               Vector3D<ptype> acceleration = s->force_slot[ci] / p_mass;
	       Vector3D<ptype> delta_v = acceleration * dt;
	       // update velocity
	       s->bead_slot[ci].velo = s->bead_slot[ci].velo + delta_v;
	       // update position 
	       s->bead_slot[ci].pos = s->bead_slot[ci].pos + s->bead_slot[ci].velo*dt + acceleration*ptype(0.5)*dt*dt;

	       // ----- clear the forces ---------------
	       s->force_slot[ci].set(ptype(0.0), ptype(0.0), ptype(0.0));

               // ----- migration code ------
	       bool migrating = false; // flag that says whether this particle needs to migrate
	       unit_t d_loc; // the potential destination for this bead

	       //    migration in the x dim
	       if(s->bead_slot[ci].pos.x() >= s->unit_size){
		       migrating = true;
		       if(s->loc.x == (N-1)){
                           d_loc.x = 0;
		       } else {
			   d_loc.x = s->loc.x + 1;
		       }
		       s->bead_slot[ci].pos.x(s->bead_slot[ci].pos.x() - s->unit_size); // make it relative to the dest
	       } else if (s->bead_slot[ci].pos.x() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.x == 0) {
			  d_loc.x = N - 1;
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
		       if(s->loc.y == (N-1)){
                           d_loc.y = 0;
		       } else {
			   d_loc.y = s->loc.y + 1;
		       }
		       s->bead_slot[ci].pos.y(s->bead_slot[ci].pos.y() - s->unit_size); // make it relative to the dest
	       } else if (s->bead_slot[ci].pos.y() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.y == 0) {
			  d_loc.y = N - 1;
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
		       if(s->loc.z == (N-1)){
                           d_loc.z = 0;
		       } else {
			   d_loc.z = s->loc.z + 1;
		       }
		       s->bead_slot[ci].pos.z(s->bead_slot[ci].pos.z() - s->unit_size); // make it relative to the dest
	       } else if (s->bead_slot[ci].pos.z() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.z == 0) {
			  d_loc.z = N - 1;
		       } else {
			  d_loc.z = s->loc.z - 1;
		       }
		       s->bead_slot[ci].pos.z(s->bead_slot[ci].pos.z() + s->unit_size); // make it relative to the dest
	       } else {
                      d_loc.z = s->loc.z;
	       }
	       
	       if(migrating) {
		  // we want to migrate bead i
		  //s->migrate[s->migratecnt++] = i;
	       }

	       i = clear_slot(i, ci);
	    }

	}
	
	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
	    uint8_t ci = get_next_slot(s->sentslot);
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
		s->sentslot = s->bslot;
		*readyToSend = No;

                // iterate over the ocupied beads twice -- and do the inter device pairwise interactions
	        uint8_t i = s->bslot;
	        while(i){
                  int ci = get_next_slot(i);
		  uint8_t j = s->bslot;
		  while(j) {
		      int cj = get_next_slot(j);
                      if(ci != cj) {
	                  if(s->bead_slot[ci].pos.dist(s->bead_slot[cj].pos) <= r_c) {
                            s->force_slot[ci] = s->force_slot[ci] + force_update(&s->bead_slot[ci], &s->bead_slot[cj]);      
			  } 
		      }
                      j = clear_slot(j,cj);
		  }
		  i = clear_slot(i, ci);
		}
	    }
	}
	
	// used to help adjust the relative positions for the periodic boundary
	inline int period_bound_adj(int dim){
            if(dim > 1) {
                  return -1;
            } else if (dim < -1) {
                  return 1;
            } else {
                  return dim;
            }
        }

	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge){
	  // Do inter device bead interactions

	  // from the device locaton get the adjustments to the bead positions
          int x_rel = period_bound_adj(msg->from.x - s->loc.x);
          int y_rel = period_bound_adj(msg->from.y - s->loc.y);
          int z_rel = period_bound_adj(msg->from.z - s->loc.z);

	  // relative position for this particle to this device
	  msg->beads[0].pos.x(msg->beads[0].pos.x() + ptype(x_rel)*s->unit_size);
	  msg->beads[0].pos.y(msg->beads[0].pos.y() + ptype(y_rel)*s->unit_size);
	  msg->beads[0].pos.z(msg->beads[0].pos.z() + ptype(z_rel)*s->unit_size);

          // loop through the occupied bead slots -- update force
	  uint8_t i = s->bslot;
	  while(i){
                 int ci = get_next_slot(i);
                 if(s->bead_slot[ci].pos.dist(msg->beads[0].pos) <= r_c){
                      s->force_slot[ci] = s->force_slot[ci] + force_update(&s->bead_slot[ci], &msg->beads[0]); 
	         }
		 i = clear_slot(i, ci);
	  }
	}

	// send to host -- sends a message to the host on termination
	inline bool sendToHost(volatile DPDMessage* msg) {
                msg->from.x = s->loc.x;
                msg->from.y = s->loc.y;
                msg->from.z = s->loc.z;
		msg->debug = get_num_beads(s->bslot);//s->bead_slot[0].velo.mag();
                msg->beads[0].pos.set(s->bead_slot[0].pos.x(), s->bead_slot[0].pos.y(), s->bead_slot[0].pos.z());
		return true;
	    //if(s->num_beads > 0) {
            //    msg->from.x = s->loc.x;
            //    msg->from.y = s->loc.y;
            //    msg->from.z = s->loc.z;
            //    msg->beads[0].type = s->bead_slot[0].type;
            //    msg->beads[0].id = s->bead_slot[0].id;
            //    msg->beads[0].pos.set(s->bead_slot[0].pos.x(), s->bead_slot[0].pos.y(), s->bead_slot[0].pos.z());
            //    msg->beads[0].velo.set(s->bead_slot[0].velo.x(), s->bead_slot[0].velo.y(), s->bead_slot[0].velo.z());
            //    return true;
	    //} else {
            //    return false;
            //}
        }

};

#endif /* _DPD_H */
