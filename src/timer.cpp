#include "timer.h"

bool isTimer(PDeviceId* timers, uint32_t numtimers, PDeviceId t) {
    for (int i = 0; i < numtimers; i++) {
        if (timers[i] == t) {
            return true;
        }
    }
    return false;
}

void timerMap(PGraph<DPDDevice, DPDState, None, DPDMessage>* g, uint32_t numPBoxesX, uint32_t numPBoxesY) {
    uint32_t numBoardsX = 3 * numPBoxesX;
    uint32_t numBoardsY = 2 * numPBoxesY;
    uint32_t numBoards = numBoardsX * numBoardsY;

    uint32_t timerNum = 0;
    PDeviceId timerIds[numBoards];

    uint32_t empty = 0;

    // Release all mapping and heap structures
    g->releaseAll();

    // Reallocate mapping structures
    g->allocateMapping();

    // Partition into subgraphs, one per board
    Placer boards(&g->graph, numBoardsX, numBoardsY);

    // Place subgraphs onto 2D mesh
    const uint32_t placerEffort = 8;
    boards.place(placerEffort);

    // For each board
    for (uint32_t boardY = 0; boardY < numBoardsY; boardY++) {
      for (uint32_t boardX = 0; boardX < numBoardsX; boardX++) {
        // Partition into subgraphs, one per mailbox
        PartitionId b = boards.mapping[boardY][boardX];
        Placer boxes(&boards.subgraphs[b],
                 TinselMailboxMeshXLen, TinselMailboxMeshYLen);
        boxes.place(placerEffort);

        bool placedTimer = false;

        // For each mailbox
        for (uint32_t mailboxX = 0; mailboxX < TinselMailboxMeshXLen; mailboxX++) {
          for (uint32_t mailboxY = 0; mailboxY < TinselMailboxMeshYLen; mailboxY++) {
            // Partition into subgraphs, one per thread
            uint32_t numThreads = 1<<TinselLogThreadsPerMailbox;
            PartitionId t = boxes.mapping[mailboxY][mailboxX];
            Placer threads(&boxes.subgraphs[t], numThreads, 1);

            // For each thread
            for (uint32_t threadNum = 0; threadNum < numThreads; threadNum++) {
              // Determine tinsel thread id
              uint32_t threadId = boardY;
              threadId = (threadId << TinselMeshXBits) | boardX;
              threadId = (threadId << TinselMailboxMeshYBits) | mailboxY;
              threadId = (threadId << TinselMailboxMeshXBits) | mailboxX;
              threadId = (threadId << (TinselLogCoresPerMailbox +
                            TinselLogThreadsPerCore)) | threadNum;

              // Get subgraph
              Graph* sg = &threads.subgraphs[threadNum];

              // Populate fromDeviceAddr mapping
              uint32_t numDevs = sg->incoming->numElems;
              if (numDevs == 0) {
                empty++;
              }
              g->numDevicesOnThread[threadId] = numDevs;
              g->fromDeviceAddr[threadId] = (PDeviceId*)
                malloc(sizeof(PDeviceId) * numDevs);
              for (uint32_t devNum = 0; devNum < numDevs; devNum++) {
                g->fromDeviceAddr[threadId][devNum] = sg->labels->elems[devNum];
                if (!placedTimer) {
                  placedTimer = true;
                  timerIds[timerNum] = sg->labels->elems[devNum];
                  timerNum++;
                }
              }
              // Populate toDeviceAddr mapping
              assert(numDevs < maxLocalDeviceId());
              for (uint32_t devNum = 0; devNum < numDevs; devNum++) {
                PDeviceAddr devAddr =
                  makeDeviceAddr(threadId, devNum);
                g->toDeviceAddr[sg->labels->elems[devNum]] = devAddr;
              }
            }
          }
        }
      }
    }

    // Reallocate and initialise heap structures
    g->allocatePartitions();
    g->initialisePartitions();
    // Tell the timers what they are, place a value in state.

    for (int i = 0; i < timerNum; i++) {
      g->devices[timerIds[i]]->state.timer = true;
    }
    std::cerr << timerNum << " timers\n";
    std::cerr << empty << " empty threads\n";
}
