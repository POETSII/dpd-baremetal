// Dissipative particle dynamics simulation POLite version
#include "parsedDPD.h"

#include <tinsel.h>
#include <POLite.h>

typedef PThread< DPDDevice, DPDState, None, DPDMessage> DPDThread;

int main() {
   // Point thread structure at the base of thread's heap
   DPDThread* thread = (DPDThread*) tinselHeapBaseSRAM();

   // Invoke interpreter
   thread->run();
}
