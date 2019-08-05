
#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <POLite.h>
#include <POLite/Placer.h>
#include "dpd.h"

void timerMap(PGraph<DPDDevice, DPDState, None, DPDMessage>* g, uint32_t numBoardsX, uint32_t numBoardsY);

#endif
