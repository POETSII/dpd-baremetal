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
#include <random>

//! generates a random position within a given space (NxN)
Vector3D<ptype> rand2DPos(unsigned N){
    Vector3D<ptype> t_pos;
    ptype x(rand() / (float)RAND_MAX * N);
    ptype y(rand() / (float)RAND_MAX * N);
    ptype z(0.0);
    t_pos.set(x,y,z);
    return t_pos;
}

Vector3D<ptype> randPos(unsigned N){
    Vector3D<ptype> t_pos;
    ptype x(rand() / (float)RAND_MAX * N);
    ptype y(rand() / (float)RAND_MAX * N);
    ptype z(rand() / (float)RAND_MAX * N);
    t_pos.set(x,y,z);
    return t_pos;
}

int main()
{

  printf("starting the DPD application\n");

  Universe<ptype> uni(problem_size, N);

  printf("Universe setup -- adding beads\n");

  uint32_t b_uid = 0;
  for(int i=0; i<90; i++) {
      bool added = false;
      while(!added) {
          bead_t *b1 = new bead_t;
          b1->id = b_uid++; 
          b1->type = 0; 
          b1->pos.set((rand() / (float)RAND_MAX * 10), (rand() / (float)RAND_MAX * 10), 0.0);
          b1->velo.set(0.0,0.0,0.0);
          if(uni.space(b1)) {
              uni.add(b1);
              added = true;
          }
      }
  }

  for(int i=0; i<40; i++) {
      bool added = false;
      while(!added) {
          bead_t *b1 = new bead_t;
          b1->id = b_uid++; 
          b1->type = 1; 
          b1->pos.set((rand() / (float)RAND_MAX * 10), (rand() / (float)RAND_MAX * 10), 0.0);
          b1->velo.set(0.0,0.0,0.0);
          if(uni.space(b1)) {
              uni.add(b1);
              added = true;
          }
      }
  }
  
  //bead_t b1;
  //b1.id = 0; 
  //b1.type = 0; 
  //b1.pos.set(8.5,0.0,0.0);
  //b1.velo.set(0.0,0.0,0.0);
  //uni.add(&b1);

  //bead_t b2;
  //b2.id = 1; 
  //b2.type = 0; 
  //b2.pos.set(1.5,0.0,0.0);
  //b2.velo.set(0.0,0.0,0.0);
  //uni.add(&b2);

  //bead_t b3;
  //b3.id = 2; 
  //b3.type = 1; 
  //b3.pos.set(1.8,0.3,0.0);
  //b3.velo.set(0.0,0.0,0.0);
  //uni.add(&b3);

  //bead_t b4;
  //b4.id = 3; 
  //b4.type = 1; 
  //b4.pos.set(7.1,0.2,0.0);
  //b4.velo.set(0.0,0.0,0.0);
  //uni.add(&b4);

  //bead_t b1;
  //b1.id = 0; 
  //b1.type = 0; 
  //b1.pos.set((rand() / (float)RAND_MAX * 4), (rand() / (float)RAND_MAX * 4), 0.0);
  //b1.velo.set(0.0,0.0,0.0);
  //uni.add(&b1);

  //bead_t b2;
  //b2.id = 1; 
  //b2.type = 0; 
  //b2.pos.set((rand() / (float)RAND_MAX * 4), (rand() / (float)RAND_MAX * 4), 0.0);
  //b2.velo.set(0.0,0.0,0.0);
  //uni.add(&b2);

  //bead_t b3;
  //b3.id = 2; 
  //b3.type = 1; 
  //b3.pos.set((rand() / (float)RAND_MAX * 4), (rand() / (float)RAND_MAX * 4), 0.0);
  //b3.velo.set(0.0,0.0,0.0);
  //uni.add(&b3);

  //bead_t b4;
  //b4.id = 3; 
  //b4.type = 1; 
  //b4.pos.set((rand() / (float)RAND_MAX * 4), (rand() / (float)RAND_MAX * 4), 0.0);
  //b4.velo.set(0.0,0.0,0.0);
  //uni.add(&b4);

  uni.write(); // write the universe into the POETS memory
  printf("running...\n");
  uni.run(); // start the simulation

  return 0;
}
