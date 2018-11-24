// some helper functions for the DPD application

#include "utils.hpp"

// inverse square root using bit twiddling cleverness (taken from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf)
// uses newton raphson
float inv_sqrt(float x) {
    float xhalf = 0.5f*x;
    int i = *(int*)&x; // get bits for floating value
    i = 0x5f3759df - (i>>1); // gives initial guess y0
    x = *(float*)&i; // convert bits back to float
    for(int i=0; i<10; i++) {
        x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    }
    return x;
}

// sqrt using the inverse square root function
float newt_sqrt(float x) {
   return 1.0/inv_sqrt(x);
}

