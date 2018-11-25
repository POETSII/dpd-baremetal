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
   unit_t loc; // the location of this cube
   bead_t beads[5]; // at most we have five beads per device
   Vector3D<ptype> force[5]; // at most 5 beads -- force for each bead
   uint8_t num_beads; // the number of beads in this device
   float unit_size; // the size of this spatial unit in one dimension

   // send tracking
   uint8_t sentcnt; // a counter used to track how many beads have been sent
}; 

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

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
		if(s->num_beads > 0)
		    *readyToSend = Pin(0);
	        else
		    *readyToSend = No;
	}
	
	// idle handler -- called once the system is idle with messages
	inline void idle() {
	    // Velocity Verlet and particle migration decisions happen here
	    for(uint8_t i=0; i<s->num_beads; i++) {
	       // ------ velocity verlet ------
               Vector3D<ptype> acceleration = s->force[i] / p_mass;
	       Vector3D<ptype> delta_v = acceleration * dt;
	       // update velocity
	       s->beads[i].velo = s->beads[i].velo + delta_v;
	       // update position 
	       s->beads[i].pos = s->beads[i].pos + s->beads[i].velo*dt + acceleration*ptype(0.5)*dt*dt;

	       // ----- clear the forces ---------------
	       s->force[i].set(ptype(0.0), ptype(0.0), ptype(0.0));

               // ----- migration code ------
	       bool migrating = false; // flag that says whether this particle needs to migrate
	       unit_t d_loc; // the potential destination for this bead

	       //    migration in the x dim
	       if(s->beads[i].pos.x() >= s->unit_size){
		       migrating = true;
		       if(s->loc.x == (N-1)){
                           d_loc.x = 0;
		       } else {
			   d_loc.x = s->loc.x + 1;
		       }
		       s->beads[i].pos.x(s->beads[i].pos.x() - s->unit_size); // make it relative to the dest
	       } else if (s->beads[i].pos.x() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.x == 0) {
			  d_loc.x = N - 1;
		       } else {
			  d_loc.x = s->loc.x - 1;
		       }
		       s->beads[i].pos.x(s->beads[i].pos.x() + s->unit_size); // make it relative to the dest
	       } else {
                      d_loc.x = s->loc.x;
	       }

	       //    migration in the y dim
	       if(s->beads[i].pos.y() >= s->unit_size){
		       migrating = true;
		       if(s->loc.y == (N-1)){
                           d_loc.y = 0;
		       } else {
			   d_loc.y = s->loc.y + 1;
		       }
		       s->beads[i].pos.y(s->beads[i].pos.y() - s->unit_size); // make it relative to the dest
	       } else if (s->beads[i].pos.y() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.y == 0) {
			  d_loc.y = N - 1;
		       } else {
			  d_loc.y = s->loc.y - 1;
		       }
		       s->beads[i].pos.y(s->beads[i].pos.y() + s->unit_size); // make it relative to the dest
	       } else {
                      d_loc.y = s->loc.y;
	       }


	       //    migration in the z dim
	       if(s->beads[i].pos.z() >= s->unit_size){
		       migrating = true;
		       if(s->loc.z == (N-1)){
                           d_loc.z = 0;
		       } else {
			   d_loc.z = s->loc.z + 1;
		       }
		       s->beads[i].pos.z(s->beads[i].pos.z() - s->unit_size); // make it relative to the dest
	       } else if (s->beads[i].pos.z() < ptype(0.0)) {
                       migrating = true;
		       if(s->loc.z == 0) {
			  d_loc.z = N - 1;
		       } else {
			  d_loc.z = s->loc.z - 1;
		       }
		       s->beads[i].pos.z(s->beads[i].pos.z() + s->unit_size); // make it relative to the dest
	       } else {
                      d_loc.z = s->loc.z;
	       }

	       
	       // ----- do we export to the host ? ---- 
	    }
	}
	
	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
	    // send all of our beads to neighbours
	    msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].type = s->beads[s->sentcnt].type;
            msg->beads[0].id = s->beads[s->sentcnt].id;
            msg->beads[0].pos.set(s->beads[s->sentcnt].pos.x(), s->beads[s->sentcnt].pos.y(), s->beads[s->sentcnt].pos.z());
            msg->beads[0].velo.set(s->beads[s->sentcnt].velo.x(), s->beads[s->sentcnt].velo.y(), s->beads[s->sentcnt].velo.z());

            // move onto the next bead to send unless we are done 
	    s->sentcnt++;
	    if(s->sentcnt < s->num_beads) { 
	        *readyToSend = Pin(0);
	    } else {
		s->sentcnt = 0;
                *readyToSend = No; 

		// perform inter-bead force updates
		for(uint8_t i=0; i<s->num_beads; i++) {
		    for(uint8_t j=0; j<s->num_beads; j++) {
                          if(i!=j) {
			     if(s->beads[i].pos.dist(s->beads[j].pos) <= r_c) {
                                  s->force[i] = s->force[i] + force_update(&s->beads[i], &s->beads[j]); 
			     } 
			  }	  
	            }
		}
	    }
	}
	
	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge){
	  // Do inter device bead interactions

	  // from the device locaton get the adjustments to the bead positions
          int x_rel = msg->from.x - s->loc.x;
          int y_rel = msg->from.y - s->loc.y;
          int z_rel = msg->from.z - s->loc.z;

          // periodic boundary adjusting
          if(x_rel > 1)
                x_rel = -1;
          else if (x_rel < -1)
                x_rel = 1;

          if(y_rel > 1)
                y_rel = -1;
          else if (y_rel < -1)
                y_rel = 1;

          if(z_rel > 1)
                z_rel = -1;
          else if (z_rel < -1)
                z_rel = 1;

	  // relative position for this particle to this device
	  msg->beads[0].pos.x(msg->beads[0].pos.x() + ptype(x_rel)*s->unit_size);
	  msg->beads[0].pos.y(msg->beads[0].pos.y() + ptype(y_rel)*s->unit_size);
	  msg->beads[0].pos.z(msg->beads[0].pos.z() + ptype(z_rel)*s->unit_size);

          for(uint8_t i=0; i < s->num_beads; i++){
              if(s->beads[i].pos.dist(msg->beads[0].pos) <= r_c){
	         // beads are in range
                 s->force[i] = s->force[i] + force_update(&s->beads[i], &msg->beads[0]); 
	      }
          }	      
	}

	// send to host -- sends a message to the host on termination
	inline bool sendToHost(volatile DPDMessage* msg) {
                msg->from.x = s->loc.x;
                msg->from.y = s->loc.y;
                msg->from.z = s->loc.z;
		msg->debug = s->beads[0].velo.mag();
                msg->beads[0].pos.set(s->beads[0].pos.x(), s->beads[0].pos.y(), s->beads[0].pos.z());
		return true;
	    //if(s->num_beads > 0) {
            //    msg->from.x = s->loc.x;
            //    msg->from.y = s->loc.y;
            //    msg->from.z = s->loc.z;
            //    msg->beads[0].type = s->beads[0].type;
            //    msg->beads[0].id = s->beads[0].id;
            //    msg->beads[0].pos.set(s->beads[0].pos.x(), s->beads[0].pos.y(), s->beads[0].pos.z());
            //    msg->beads[0].velo.set(s->beads[0].velo.x(), s->beads[0].velo.y(), s->beads[0].velo.z());
            //    return true;
	    //} else {
            //    return false;
            //}
        }

};

#endif /* _DPD_H */
