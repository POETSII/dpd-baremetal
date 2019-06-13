#include "timer.h"

bool isTimer(PDeviceId* timers, uint32_t numtimers, PDeviceId t) {
    for (int i = 0; i < numtimers; i++) {
        if (timers[i] == t) {
            return true;
        }
    }
    return false;
}

void timerMap(PGraph<DPDDevice, DPDState, None, DPDMessage>* g) {
    uint32_t numBoardsX = 3;
    uint32_t numBoardsY = 2;

    PDeviceId timers[numBoardsX * numBoardsY];
    uint32_t timers_placed = 0;
    for (int i = 0; i < (numBoardsX * numBoardsY); i++) {
        timers[i] = g->newDevice();
        if (i > 0) {
            g->addEdge(timers[0], 0, timers[i]);
        }
    }
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

        // For each mailbox
        for (uint32_t mailboxX = 0; mailboxX < TinselMailboxMeshXLen; mailboxX++) {
          for (uint32_t mailboxY = 0; mailboxY < TinselMailboxMeshYLen; mailboxY++) {
            // Partition into subgraphs, one per thread
            uint32_t numThreads = 1<<TinselLogThreadsPerMailbox;
            PartitionId t = boxes.mapping[mailboxY][mailboxX];
            Placer threads(&boxes.subgraphs[t], numThreads, 1);

            // Remove previously placed timers
            for (uint32_t threadNum = 0; threadNum < numThreads; threadNum++) {
                uint32_t threadId = boardY;
                threadId = (threadId << TinselMeshXBits) | boardX;
                threadId = (threadId << TinselMailboxMeshYBits) | mailboxY;
                threadId = (threadId << TinselMailboxMeshXBits) | mailboxX;
                threadId = (threadId << (TinselLogCoresPerMailbox +
                            TinselLogThreadsPerCore)) | threadNum;

                Graph* sg = &threads.subgraphs[threadNum];

                uint32_t numDevs = sg->incoming->numElems;
                for (int32_t devNum = 0; devNum < numDevs; devNum++) {
                    if (isTimer(timers, (numBoardsX * numBoardsY), sg->labels->elems[devNum])) {
                        sg->incoming->remove(sg->incoming->elems[devNum]);
                        sg->outgoing->remove(sg->outgoing->elems[devNum]);
                        sg->pins->remove(sg->pins->elems[devNum]);
                        sg->edgeLabels->remove(sg->edgeLabels->elems[devNum]);
                        sg->labels->remove(devNum);
                    }
                }
            }

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

              // If first thread on board, add timer
              if (threadNum == 0 && mailboxX == 0 && mailboxY == 0) {
                NodeId t = sg->newNode();
                sg->setLabel(t, timers[timers_placed]);
                timers_placed++;
              }

              // Populate fromDeviceAddr mapping
              uint32_t numDevs = sg->incoming->numElems;
              g->numDevicesOnThread[threadId] = numDevs;
              g->fromDeviceAddr[threadId] = (PDeviceId*)
                malloc(sizeof(PDeviceId) * numDevs);
              for (uint32_t devNum = 0; devNum < numDevs; devNum++)
                g->fromDeviceAddr[threadId][devNum] = sg->labels->elems[devNum];
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
    g->devices[timers[0]]->state.timer = 1;
    for (int i = 1; i < 6; i++) {
      g->devices[timers[i]]->state.timer = 2;
    }
}
