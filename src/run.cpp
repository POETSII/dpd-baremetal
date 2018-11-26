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

int main()
{

  printf("starting the DPD application\n");

  Universe<ptype> uni(problem_size, N);

  printf("Universe setup -- adding beads\n");

  // placing beads into the universe 
  bead_t b1;
  b1.id = 2; 
  b1.type = 0; 
  b1.pos.set(0.0,0.0,1.95);
  b1.velo.set(0.0,0.0,0.0);
  uni.add(&b1);

  bead_t b2;
  b2.id = 1; 
  b2.type = 0; 
  b2.pos.set(0.0,0.0,1.99);
  b2.velo.set(0.0,0.0,0.0);
  uni.add(&b2);

  //bead_t b3;
  //b3.id = 3; 
  //b3.type = 0; 
  //b3.pos.set(4.0,7.7,4.3);
  //b3.velo.set(0.0,0.0,0.0);
  //uni.add(&b3);

  uni.write(); // write the universe into the POETS memory
  printf("running...\n");
  uni.run(); // start the simulation

  return 0;
}
