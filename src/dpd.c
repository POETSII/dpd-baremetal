// Dissipative particle dynamics simulation

#include <tinsel.h>
#include "dpd.h"

// I need a static way to convienently write fixed point constants
// This needs to be precomputed... also it will overflow the current fixed point type...
const fix16_t A[3][3] = { {25.0, 75.0, 35.0},
                          {75.0, 25.0, 50.0},
                          {35.0, 50.0, 25.0} }; // interaction matrix

// assumption r_c is 1.0
// function used to update the force of two particles
vec_t forces(vec_t apos, vec_t bpos, vec_t avelo, vec_t bvelo){
     
}

// -------------------------
// Three phases
// -------------------------
//
// 2. Update phase
// -------------------------
// Each thread shares the position of their beads for this timestep.
// When beads are received from neighbours if the beads are in range
// they are used to update the current force of that bead. 
// Once a message from all neighbours have been received then we
// update the position of our beads using verlocity verlet and send 
// the location of our beads to our neighbours
// 
//  --- wait for idle ---
//
// 3. Migration phase
// -------------------------
// All migrated particles are exchanged between the threads - only when needed
//
// back to stage 1

// Top-level
// ---------
int main()
{
 
  // ----------------------------------------
  //  Receive a message and update position
  // ----------------------------------------
   

  return 0;
}
