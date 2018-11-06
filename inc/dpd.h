// This header file is shared by the tinsel program and the host PC program
// It is used to define various DPD particles

#ifndef _DPD_H_
#define _DPD_H_

#define BEADS_PER_UNIT 5 
#define UNIT_SPACE 1
#define PADDING 

// position type
typedef uint16_t pos_t; 

// bead class 
typedef uint16_t bead_class_t; 

// bead ID
typedef uint16_t bead_id_t; 

// unit location
typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} unit_t;	

// defines a bead type
typedef struct {
  bead_id_t id; 
  bead_class_t type; 
  pos_t x; 
  pos_t y; 
  pos_t z;
} bead_t; 


// Format of messages sent to host
typedef struct {
  unit_t from; // the unit that this message is from
  uint8_t count; // the number of beads in this message
  bead_t beads[BEADS_PER_UNIT]; // the beads from this unit 
  uint32_t timestep; // the timestep at this message
  //uint8_t padding[4];
} msg_t;

#endif /* _DPD_H */
