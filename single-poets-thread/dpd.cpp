// SPDX-License-Identifier: BSD-2-Clause

#include <tinsel.h>
#include "dpd.h"

uint32_t get_next_free_slot(uint32_t slotlist) {
    uint32_t mask = 0x1;
    for(int i=0; i<31; i++){
        if(!(slotlist & mask)) {
           return i;
        }
        mask = mask << 1;
    }
    return 0xFFFFFFFF; // error there are no free slots!
}

uint32_t set_slot(uint32_t slotlist, uint32_t pos){ return slotlist | (1 << pos); }

uint32_t get_next_slot(uint32_t slotlist) {
    uint32_t mask = 0x1;
    for(int i=0; i<31; i++) {
        if(slotlist & mask) {
            return i;
        }
        mask = mask << 1; // shift to the next pos
    }
    return 0xFFFFFFFF; // we are empty
}

uint32_t clear_slot(uint32_t slotlist, uint32_t pos){  return slotlist & ~(1 << pos);  }

int main()
{
    cell_t cells[VOL_SIZE][VOL_SIZE][VOL_SIZE];
    tinselSetLen(3);
// INIT
    // Get message slot
    volatile HostMsg* msg = (HostMsg*) tinselSlot(15);
    // Send to host to end
    uint32_t hostId = tinselHostId();
    // Initialise cell locations
    for (uint16_t x = 0; x < VOL_SIZE; x++) {
        for (uint16_t y = 0; y < VOL_SIZE; y++) {
            for (uint16_t z = 0; z < VOL_SIZE; z++) {
                cells[x][y][z].loc = {x, y, z};
                cells[x][y][z].bslot = 0;
                for (int i = 0; i < MAX_BEADS; i++) {
                    cells[x][y][z].bead_slot[i].id = 0;
                    cells[x][y][z].bead_slot[i].type = 0;
                    cells[x][y][z].bead_slot[i].pos.x(0);
                    cells[x][y][z].bead_slot[i].pos.y(0);
                    cells[x][y][z].bead_slot[i].pos.z(0);
                    cells[x][y][z].bead_slot[i].velo.x(0);
                    cells[x][y][z].bead_slot[i].velo.y(0);
                    cells[x][y][z].bead_slot[i].velo.z(0);
                }
            }
        }
    }

    // Recv beads from host

    // Get pointers to mailbox message slots
    volatile int* msgIn = (int*) tinselSlot(0);

    uint32_t beads_added = 0;
    while (true) {
        tinselAlloc(msgIn);
        tinselWaitUntil(TINSEL_CAN_RECV);
        volatile HostMsg* msg = (HostMsg*) tinselRecv();
        if (msg->type == 0xAA) {
            break;
        } else if (msg->type == 0x0A) {

            // Add bead to cells state
            // Get location (for array addressing)
            unit_t loc;
            loc.x = msg->from.x;
            loc.y = msg->from.y;
            loc.z = msg->from.z;

            // Get current bitmap
            uint32_t bslot = cells[loc.x][loc.y][loc.z].bslot;
            // Find next free slot
            uint32_t new_slot = get_next_free_slot(bslot);
            // Does it have space?
            if (new_slot == 0xFFFFFFFF) {
                return 2;
            }

            bead_t b;
            b.id = msg->beads[0].id;
            b.type = msg->beads[0].type;
            b.pos.x(msg->beads[0].pos.x());
            b.pos.y(msg->beads[0].pos.y());
            b.pos.z(msg->beads[0].pos.z());
            b.velo.x(msg->beads[0].velo.x());
            b.velo.y(msg->beads[0].velo.y());
            b.velo.z(msg->beads[0].velo.z());

            // Add it to this slot
            bslot = set_slot(bslot, new_slot);
            cells[loc.x][loc.y][loc.z].bslot = bslot;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].id = b.id;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].type = b.type;
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.x(b.pos.x());
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.y(b.pos.y());
            cells[loc.x][loc.y][loc.z].bead_slot[new_slot].pos.z(b.pos.z());

            beads_added++;
        }
    }

// Pass beads back to host to check they're stored correctly
    for (uint16_t x = 0; x < VOL_SIZE; x++) {
        for (uint16_t y = 0; y < VOL_SIZE; y++) {
            for (uint16_t z = 0; z < VOL_SIZE; z++) {
                uint32_t bslot = cells[x][y][z].bslot;
                while (bslot) {
                    msg->type = 0x0B;
                    msg->from.x = cells[x][y][z].loc.x;
                    msg->from.y = cells[x][y][z].loc.y;
                    msg->from.z = cells[x][y][z].loc.z;

                    uint32_t ci = get_next_slot(bslot);

                    // msg->beads[0].id = cells[x][y][z].bead_slot[ci].id;
                    msg->beads[0].id = cells[x][y][z].bead_slot[ci].id;
                    msg->beads[0].type = cells[x][y][z].bead_slot[ci].type;
                    msg->beads[0].pos.x(cells[x][y][z].bead_slot[ci].pos.x());
                    msg->beads[0].pos.y(cells[x][y][z].bead_slot[ci].pos.y());
                    msg->beads[0].pos.z(cells[x][y][z].bead_slot[ci].pos.z());
                    msg->beads[0].velo.x(cells[x][y][z].bead_slot[ci].velo.x());
                    msg->beads[0].velo.y(cells[x][y][z].bead_slot[ci].velo.y());
                    msg->beads[0].velo.z(cells[x][y][z].bead_slot[ci].velo.z());

                    bslot = clear_slot(bslot, ci);

                    // Wait until we can send
                    tinselWaitUntil(TINSEL_CAN_SEND);
                    // Send to host
                    tinselSend(hostId, msg);
                }
            }
        }
    }

// FINISH
    // Message to be sent to the host
    msg->type = 0xBB;

    // Wait until we can send
    tinselWaitUntil(TINSEL_CAN_SEND);

    tinselSend(hostId, msg);

  return 0;
}
