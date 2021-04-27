#ifndef _EXTERNALS_COMMON_H
#define _EXTERNALS_COMMON_H
#ifdef GALS
#include "gals.h"
#elif defined(SERIAL)
#include "SerialSimulator.hpp"
#else
#include "sync.h"
#endif

typedef struct _pts_to_extern_t {
   uint32_t timestep;
   cell_t from;
   bead_t bead;
} pts_to_extern_t;

#endif /* _EXTERNALS_COMMON_H */
