#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"
#include <map>
#include <math.h>

const float problem_size = 10.0; // total size of the sim universe in one dimension
const unsigned N = 10; // the size of the sim universe in each dimension
const uint8_t max_beads_per_dev = 5; // the maximum number of beads permissible in a device

// when given a bead return the SU that it should be assigned to
// the position of the particle is the global position for the simulation -- not the local position for the device
PDeviceId getBeadSU(bead_t *in, std::map<unit_t, PDeviceId> *locToId, ptype unit_size){
    Vector3D<ptype> pos = in->pos; 
    unit_pos_t x = floor(pos.x()/unit_size); 
    unit_pos_t y = floor(pos.y()/unit_size); 
    unit_pos_t z = floor(pos.z()/unit_size); 
    unit_t t = {x,y,z};
    // from the unit_t address lookup the device ID
    return (*locToId)[t]; 
}

int main()
{

  printf("starting the DPD application\n");
  
  HostLink hostLink;

  // Create a POETS graph
  PGraph<DPDDevice, DPDState, None, DPDMessage> graph;

  // maintain a map of ID's to locations in the space
  std::map<PDeviceId, unit_t> idToLoc;
  std::map<unit_t, PDeviceId> locToId;

  // size for each unit (in one dimension -- assumes a cube)
  float unit_size = problem_size/(float)N; 

  // Add a test device
  for(uint16_t x=0; x<N; x++) { 
       for(uint16_t y=0; y<N; y++) { 
             for(uint16_t z=0; z<N; z++) { 
                    PDeviceId id = graph.newDevice();
		    unit_t loc = {x, y, z};
		    idToLoc[id] = loc; 
		    locToId[loc] = id; 
	     }
	}
   }

  // map the graph into the hardware
  graph.map();

  // ---------------------------------------------
  //  Device initialisation
  // ---------------------------------------------
  // add the location to event device in the space
  for(std::map<PDeviceId, unit_t>::iterator i = idToLoc.begin(); i!=idToLoc.end(); ++i) {
      PDeviceId cId = i->first;
      unit_t loc = i->second;
      graph.devices[cId]->state.loc.x = loc.x;
      graph.devices[cId]->state.loc.y = loc.y;
      graph.devices[cId]->state.loc.z = loc.z;
      graph.devices[cId]->state.unit_size = unit_size;
  }

  // placing beads into the universe 
  bead_t b1;
  b1.id = 0; 
  b1.type = 0; 
  b1.pos.set(4.5,3.6,8.2);
  b1.velo.set(0.0,0.0,0.0);
  PDeviceId b1_su = getBeadSU(&b1, &locToId, unit_size); 
  // allocate the bead
  graph.devices[b1_su]->state.beads[graph.devices[b1_su]->state.num_beads++] = b1;

  // placing beads into the universe 
  bead_t b2;
  b2.id = 0; 
  b2.type = 0; 
  b2.pos.set(3.6,2.9,1.2);
  b2.velo.set(0.0,0.0,0.0);
  PDeviceId b2_su = getBeadSU(&b2, &locToId, unit_size); 
  // allocate the bead
  graph.devices[b2_su]->state.beads[graph.devices[b2_su]->state.num_beads++] = b2;

  // ---------------------------------------------

  // load the graph onto the hardware
  graph.write(&hostLink);

  // Load application
  hostLink.boot("code.v", "data.v");

  // Start timer
  struct timeval start, finish, diff;
  gettimeofday(&start, NULL);

  // Start application
  hostLink.go();

  // Stop timer
  gettimeofday(&finish, NULL);
 
  // Display time
  timersub(&finish, &start, &diff);
  double duration = (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
  PMessage<None, DPDMessage> msg;
  for(uint32_t i=0; i< 2; i++) {
      hostLink.recvMsg(&msg, sizeof(msg));
      printf("<%d,%d,%d> has bead <%.4f,%.4f,%.4f>\n", msg.payload.from.x, msg.payload.from.y, msg.payload.from.z, msg.payload.beads[0].pos.x(), msg.payload.beads[0].pos.y(), msg.payload.beads[0].pos.z());
  }
  printf("Time = %lf\n", duration);

  return 0;
}
