#include "timer.h"

void timerMap(PGraph<DPDDevice, DPDState, None, DPDMessage>* g) {
    uint32_t numBoardsX = 3;
    uint32_t numBoardsY = 2;

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
        for (uint32_t boxX = 0; boxX < TinselMailboxMeshXLen; boxX++) {
          for (uint32_t boxY = 0; boxY < TinselMailboxMeshYLen; boxY++) {
            // Partition into subgraphs, one per thread
            uint32_t numThreads = 1<<TinselLogThreadsPerMailbox;
            PartitionId t = boxes.mapping[boxY][boxX];
            Placer threads(&boxes.subgraphs[t], numThreads, 1);

            // For each thread
            for (uint32_t threadNum = 0; threadNum < numThreads; threadNum++) {
              // Determine tinsel thread id
              uint32_t threadId = boardY;
              threadId = (threadId << TinselMeshXBits) | boardX;
              threadId = (threadId << TinselMailboxMeshYBits) | boxY;
              threadId = (threadId << TinselMailboxMeshXBits) | boxX;
              threadId = (threadId << (TinselLogCoresPerMailbox +
                            TinselLogThreadsPerCore)) | threadNum;

              // Get subgraph
              Graph* sg = &threads.subgraphs[threadNum];

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
}
