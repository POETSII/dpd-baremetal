#ifndef _EXTERNALS_COMMON_H
#define _EXTERNALS_COMMON_H
#ifndef GALS
#include "sync.h"
#else
#include "gals.h"
#endif

typedef struct _pts_to_extern_t {
   uint32_t timestep;
   cell_t from;
   bead_t bead;
} pts_to_extern_t;

#endif /* _EXTERNALS_COMMON_H */
