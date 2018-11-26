#ifndef _EXTERNALS_COMMON_H
#define _EXTERNALS_COMMON_H
#include "dpd.h"

typedef struct _pts_to_extern_t {
   uint32_t timestep;
   unit_t from;
   bead_t bead;
} pts_to_extern_t;

#endif /* _EXTERNALS_COMMON_H */
