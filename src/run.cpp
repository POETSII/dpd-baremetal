#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"

int main()
{
  HostLink hostLink;

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
  printf("Time = %lf\n", duration);

  return 0;
}
