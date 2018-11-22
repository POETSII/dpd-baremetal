#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"
#include <map>

const unsigned N = 10; // the size of the sim universe in each dimension

int main()
{

  printf("starting the DPD application\n");
  
  HostLink hostLink;

  // Create a POETS graph
  PGraph<DPDDevice, DPDState, None, DPDMessage> graph;

  // maintain a map of ID's to locations in the space
  std::map<PDeviceId, unit_t> idToLoc;

  // Add a test device
  for(uint16_t x=0; x<N; x++) { 
       for(uint16_t y=0; y<N; y++) { 
             for(uint16_t z=0; z<N; z++) { 
                    PDeviceId id = graph.newDevice();
		    unit_t loc = {x, y, z};
		    idToLoc[id] = loc; 
	     }
	}
   }

  // map the graph into the hardware
  graph.map();

  // add the location to event device in the space
  for(std::map<PDeviceId, unit_t>::iterator i = idToLoc.begin(); i!=idToLoc.end(); ++i) {
      PDeviceId cId = i->first;
      unit_t loc = i->second;
      graph.devices[cId]->state.loc.x = loc.x;
      graph.devices[cId]->state.loc.y = loc.y;
      graph.devices[cId]->state.loc.z = loc.z;
  }

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
  for(uint32_t i=0; i< graph.numDevices; i++) {
      hostLink.recvMsg(&msg, sizeof(msg));
      printf("<%d,%d,%d> = %.4f,\n", msg.payload.from.x, msg.payload.from.y, msg.payload.from.z, msg.payload.debug);
  }
  printf("Time = %lf\n", duration);

  return 0;
}
