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

// particle location
typedef struct _pos_t {
    float x;
    float y;
    float z;
} pos_t; // 12 bytes

// particle velocity
typedef struct _velo_t {
    float x;
    float y;
    float z;
} velo_t; // 12 bytes

typedef uint16_t bead_class_t; // the type of the bead, we are not expecting too many 
typedef uint16_t bead_id_t; // the ID for the bead

// defines a bead type
typedef struct _bead_t {
    bead_class_t type;
    bead_id_t id;
    pos_t pos;
    velo_t velo;
} bead_t; // 28 bytes 

typedef uint16_t unit_pos_t;

// defines the unit location
typedef struct _unit_t {
  unit_pos_t x;
  unit_pos_t y;
  unit_pos_t z; 
} unit_t; // 6 bytes

// Format of message
struct DPDMessage {
  float debug;
  unit_t from; // the unit that this message is from 
  //bead_t beads[BEADS_PER_UNIT]; // the beads from this unit 
}; 

// the state of the DPD Device
struct DPDState{
   unit_t loc; // the location of this cube
   Vector3D<ptype> pos1;
   Vector3D<ptype> pos2;
}; 

// DPD Device code
struct DPDDevice : PDevice<DPDState, None, DPDMessage> {

	// init handler -- called once by POLite at the start of execution
	inline void init() {
             s->pos1 = Vector3D<ptype>(ptype(1.2), ptype(3.4), ptype(4.5));		
             s->pos2 = Vector3D<ptype>(ptype(2.3), ptype(3.0), ptype(1.0));		
	}
	
	// idle handler -- called once the system is idle with messages
	inline void idle() {

	}
	
	// send handler -- called when the ready to send flag has been set
	inline void send(volatile DPDMessage *msg){

	}
	
	// recv handler -- called when the device has received a message
	inline void recv(DPDMessage *msg, None* edge){

	}

	// send to host -- sends a message to the host on termination
	inline bool sendToHost(volatile DPDMessage* msg) {
	    msg->debug = s->pos1.dot(s->pos2); 
            msg->from.x = s->loc.x;
            msg->from.y = s->loc.y;
            msg->from.z = s->loc.z;
            return true;
        }

};

#endif /* _DPD_H */
