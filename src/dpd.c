// Dissipative particle dynamics simulation

#include <tinsel.h>
#include "dpd.h"

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
