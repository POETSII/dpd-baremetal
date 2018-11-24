#ifndef __UTILS_H
#define __UTILS_H

// some helper functions for this class
// inverse square root using bit twiddling cleverness (taken from http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf)
// uses newton raphson
float inv_sqrt(float x);

// sqrt using the inverse square root function
float newt_sqrt(float x); 

#endif /* _UTILS_H */
