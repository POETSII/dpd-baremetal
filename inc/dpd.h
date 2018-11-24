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
}; 

const ptype r_c(1.0);

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

	// init handler -- called once by POLite at the start of execution
	inline void init() {
		s->debug_cnt = 0.0;
		if(s->num_beads > 0)
		    *readyToSend = Pin(0);
	}
	
	// idle handler -- called once the system is idle with messages
	inline void idle() {

	}
	
	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){
	    // Do intra device bead interations
	    msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            msg->beads[0].type = s->beads[0].type;
            msg->beads[0].id = s->beads[0].id;
            msg->beads[0].pos.set(s->beads[0].pos.x(), s->beads[0].pos.y(), s->beads[0].pos.z());
            msg->beads[0].velo.set(s->beads[0].velo.x(), s->beads[0].velo.y(), s->beads[0].velo.z());
            *readyToSend = No; 
	}
	
	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge){
	  // Do inter device bead interactions

	  // from the message get the adjustments to the position
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
	  Vector3D<ptype> npos;
	  npos.x(msg->beads[0].pos.x() + ptype(x_rel)*s->unit_size);
	  npos.y(msg->beads[0].pos.y() + ptype(y_rel)*s->unit_size);
	  npos.z(msg->beads[0].pos.z() + ptype(z_rel)*s->unit_size);

          for(uint8_t i=0; i < s->num_beads; i++){
              if(s->beads[i].pos.dist(npos) <= r_c){
	         // beads are in range
                 s->debug_cnt = s->beads[i].pos.dist(npos); 
	      }
          }	      
	}

	// send to host -- sends a message to the host on termination
	inline bool sendToHost(volatile DPDMessage* msg) {
                msg->from.x = s->loc.x;
                msg->from.y = s->loc.y;
                msg->from.z = s->loc.z;
		msg->debug = s->debug_cnt;
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
