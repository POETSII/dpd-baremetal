#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"

int main()
{

  printf("size of bead = %lu\n", sizeof(bead_t));
  printf("size of message = %lu\n", sizeof(DPDMessage));

  HostLink hostLink;

  // Create a POETS graph
  PGraph<DPDDevice, DPDState, None, DPDMessage> graph;

  // Add a test device
  PDeviceId id = graph.newDevice();

  // map the graph into the hardware
  graph.map();

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
  hostLink.recvMsg(&msg, sizeof(msg));
  unit_t from = msg.payload.from;
  printf("%d,%d,%d\n", from.x, from.y, from.z);
  printf("Time = %lf\n", duration);

  return 0;
}
