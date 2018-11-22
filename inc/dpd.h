// This header file is shared by the tinsel program and the host PC program
// It is used to define various DPD particles

#include <stdint.h>

#ifndef _DPD_H_
#define _DPD_H_

#define BEADS_PER_UNIT 2 
#define UNIT_SPACE 1
#define PADDING 

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

// Format of messages sent to host
typedef struct _msg_t {
  unit_t from; // the unit that this message is from 
  bead_t beads[BEADS_PER_UNIT]; // the beads from this unit 
} msg_t; 

#endif /* _DPD_H */
