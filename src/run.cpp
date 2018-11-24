#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"
#include "universe.hpp"
#include <map>
#include <math.h>

const float problem_size = 10.0; // total size of the sim universe in one dimension
const unsigned N = 10; // the size of the sim universe in each dimension

int main()
{

  printf("starting the DPD application\n");

  Universe<ptype> uni(problem_size, N);

  // placing beads into the universe 
  bead_t b1;
  b1.id = 0; 
  b1.type = 0; 
  b1.pos.set(4.5,3.6,8.2);
  b1.velo.set(0.0,0.0,0.0);
  uni.add(&b1);

  // placing beads into the universe 
  bead_t b2;
  b2.id = 0; 
  b2.type = 0; 
  b2.pos.set(3.6,2.9,1.2);
  b2.velo.set(0.0,0.0,0.0);
  uni.add(&b2);

  uni.write(); // write the universe into the POETS memory
  uni.run(); // start the simulation

  return 0;
}
