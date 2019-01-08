// some helper functions for the DPD application

#include "utils.hpp"

// inverse square root using bit twiddling cleverness (taken from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf)
// uses newton raphson
float inv_sqrt(float x) {
    float xhalf = 0.5f*x;
    int i = *(int*)&x; // get bits for floating value
    i = 0x5f3759df - (i>>1); // gives initial guess y0
    x = *(float*)&i; // convert bits back to float
    for(int i=0; i<5; i++) {
        x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    }
    return x;
}

// sqrt using the inverse square root function
float newt_sqrt(float x) {
   //return 1.0/inv_sqrt(x);
   
   // https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
   // We have more integer than float isue bandwidth, plus fast divides (well, same speed as anything else),
   // so might as well use this one.
   
   //tinselAssert(x>=0);
   
   union
   {
       int i;
       float x;
   } u;
   u.x = x;
   u.i = (1<<29) + (u.i >> 1) - (1<<22);
   
   // Two Babylonian Steps (simplified from:)
   // u.x = 0.5f * (u.x + x/u.x);
   // u.x = 0.5f * (u.x + x/u.x);
   
   //u.x =       u.x + x/u.x;
   //u.x = 0.25f*u.x + x/u.x;
   
   u.x = 0.5f * (u.x + x/u.x);
   u.x = 0.5f * (u.x + x/u.x);
   u.x = 0.5f * (u.x + x/u.x);
   
   return u.x; 
}

