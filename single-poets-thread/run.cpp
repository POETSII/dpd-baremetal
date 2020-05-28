// SPDX-License-Identifier: BSD-2-Clause
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include <HostLink.h>
#include "dpd.h"
#include "Vector3D.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <map>

const uint8_t max_beads_per_dev = 7;

const int total_beads = VOL_SIZE * VOL_SIZE * VOL_SIZE * BEAD_DENSITY;
const int w = 0.6 * total_beads;
const int r = 0.3 * total_beads;
const int o = 0.1 * total_beads;

int beads_added = 0;

uint32_t get_num_beads(uint32_t slotlist) {
    uint32_t cnt = 0;
    uint32_t mask = 0x1;
    for(int i=0; i<8; i++) {
        if(slotlist & mask) {
            cnt++;
        }
        mask = mask << 1;
    }
    return cnt; // error there are no free slots!
}

bool space(const bead_t *in, std::map<unit_t, cell_t>* cells) {
    bead_t b = *in;
    unit_pos_t x = floor(b.pos.x()/1);
    unit_pos_t y = floor(b.pos.y()/1);
    unit_pos_t z = floor(b.pos.z()/1);
    unit_t t = {x, y, z};

    if (x > VOL_SIZE || x < 0 || y > VOL_SIZE || y < 0 || z > VOL_SIZE   || z < 0) {
        return false;
    }

    // lookup the device
    cell_t c = cells->at(t);

    // check to make sure there is still enough room in the device
    if(get_num_beads(c.bslot) >= (max_beads_per_dev)) {
        return false;
    } else {
        return true;
    }
}

uint32_t get_next_free_slot(uint32_t slotlist) {
    uint32_t mask = 0x1;
    for(int i=0; i<31; i++){
        if(!(slotlist & mask)) {
           return i;
        }
        mask = mask << 1;
    }
    return 0xF; // error there are no free slots!
}

uint32_t set_slot(uint32_t slotlist, uint32_t pos){ return slotlist | (1 << pos); }

unit_t add(const bead_t *in, std::map<unit_t, cell_t>* cells) {
    bead_t b = *in;
    unit_pos_t x = floor(b.pos.x()/1);
    unit_pos_t y = floor(b.pos.y()/1);
    unit_pos_t z = floor(b.pos.z()/1);
    unit_t t = {x, y, z};

    // lookup the device
    cell_t c = cells->at(t);

    // check to make sure there is still enough room in the device
    if(get_num_beads(c.bslot) > max_beads_per_dev) {
        // printf("Error: there is not enough space in device:%d for bead:%d  already %u beads in the slot\n", b_su, in->id, get_num_beads(_g->devices[b_su]->state.bslot));
        fflush(stdout);
        exit(EXIT_FAILURE);
    } else {
        // we can add the bead

        // make the postion of the bead relative
        b.pos.x(b.pos.x() - (float(x)*1));
        b.pos.y(b.pos.y() - (float(y)*1));
        b.pos.z(b.pos.z() - (float(z)*1));

        // get the next free slot in this device
        uint32_t slot = get_next_free_slot(c.bslot);
        c.bead_slot[slot] = b;
        c.bslot = set_slot(c.bslot, slot);
    }

    cells->at(t) = c;

    return t;
}

void generateBeads(std::map<unit_t, cell_t>* cells) {

    uint32_t b_uid = 0;
    for(int i=0; i<w; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 0;
            b1->pos.set((rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE));
            b1->velo.set(0.0,0.0,0.0);
            if(space(b1, cells)) {
                unit_t u = add(b1, cells);
                added = true;
                beads_added++;
            }
        }
    }

    for(int i=0; i<r; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 1;
            b1->pos.set((rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE));
            b1->velo.set(0.0,0.0,0.0);
            if(space(b1, cells)) {
                add(b1, cells);
                added = true;
                beads_added++;
            }
        }
    }

    for(int i=0; i<o; i++) {
        bool added = false;
        while(!added) {
            bead_t *b1 = new bead_t;
            b1->id = b_uid++;
            b1->type = 2;
            b1->pos.set((rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE), (rand() / (float)RAND_MAX * VOL_SIZE));
            b1->velo.set(0.0,0.0,0.0);
            if(space(b1, cells)) {
                add(b1, cells);
                added = true;
                beads_added++;
            }
        }
    }
}

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

// void test() {
    // std::map<unit_t, std::map<uint32_t, bead_t>> bead_map;

    // uint32_t beads_recv = 0;

    // while (true) {
    //     // Get message from thread
    //     hostLink.recvMsg(&msg, sizeof(HostMsg));
    //     if (msg.type == 0xBB) {
    //       break;
    //     }
    //     beads_recv++;

    //     unit_t t;
    //     t.x = msg.from.x;
    //     t.y = msg.from.y;
    //     t.z = msg.from.z;

    //     uint32_t bead_id = msg.beads[0].id;
    //     bead_map[t][bead_id] = msg.beads[0];
    // }

    // std::cerr << "Beads recv = " << beads_recv << "\n";

//     bool overall_fail = false;

//     for (std::map<unit_t, cell_t>::iterator i = cells.begin(); i != cells.end(); ++i) {
//         unit_t t = i->first;
//         cell_t c = i->second;

//         uint32_t bslot = c.bslot;
//         while (bslot) {
//             uint32_t ci = get_next_slot(bslot);

//             bead_t sentBead = c.bead_slot[ci];

//             bead_t recvBead = bead_map[t][sentBead.id];

//             std::cerr << "Bead id: sent = " << sentBead.id << ", recv = " << recvBead.id;
//             if (sentBead.id == recvBead.id) {
//                 std::cerr << " - PASS\n";
//             } else {
//                 std::cerr << " - FAIL\n";
//                 overall_fail = true;
//             }

//             std::cerr << "Bead type: sent = " << sentBead.type << ", recv = " << recvBead.type;
//             if (sentBead.type == recvBead.type) {
//                 std::cerr << " - PASS\n";
//             } else {
//                 std::cerr << " - FAIL\n";
//                 overall_fail = true;
//             }

//             std::cerr << "Bead pos: sent = (" << sentBead.pos.x() << ", " << sentBead.pos.y() << ", " << sentBead.pos.z() << ")\n";
//             std::cerr << "          recv = (" << recvBead.pos.x() << ", " << recvBead.pos.y() << ", " << recvBead.pos.z() << ")\n";

//             if (sentBead.pos.x() == recvBead.pos.x() && sentBead.pos.y() == recvBead.pos.y() && sentBead.pos.z() == recvBead.pos.z()) {
//                 std::cerr << "PASS\n";
//             } else {
//                 std::cerr << "FAIL\n";
//                 overall_fail = true;
//             }

//             std::cerr << "Bead velo: sent = (" << sentBead.velo.x() << ", " << sentBead.velo.y() << ", " << sentBead.velo.z() << ")\n";
//             std::cerr << "           recv = (" << recvBead.velo.x() << ", " << recvBead.velo.y() << ", " << recvBead.velo.z() << ")\n";

//             if (sentBead.velo.x() == recvBead.velo.x() && sentBead.velo.y() == recvBead.velo.y() && sentBead.velo.z() == recvBead.velo.z()) {
//                 std::cerr << "PASS\n";
//             } else {
//                 std::cerr << "FAIL\n";
//                 overall_fail = true;
//             }

//             bslot = clear_slot(bslot, ci);
//         }
//     }

//     if (overall_fail) {
//         std::cerr << "TESTING HAS FAILED\n";
//     } else {
//         std::cerr << "TESTING HAS PASSED\n";
//     }
// }

int main()
{
    // Hold the cells here
    std::map<unit_t, cell_t> cells;

    // Set up cells for generating beads
    for (uint16_t x = 0; x < VOL_SIZE; x++) {
        for (uint16_t y = 0; y < VOL_SIZE; y++) {
            for (uint16_t z = 0; z < VOL_SIZE; z++) {
                cell_t cell;
                cell.loc = {x, y, z};
                cell.bslot = 0;
                cells[cell.loc] = cell;
            }
        }
    }

    // Generate beads
    generateBeads(&cells);
    std::cerr << beads_added << " beads generated\n";

  // Acquire hostlink
  HostLink hostLink;
  std::cerr << "Host link acquired\n";

  // Load application
  hostLink.boot("code.v", "data.v");
  std::cerr << "Host link booted\n";

  // Start timer
  struct timeval start, finish, diff;
  gettimeofday(&start, NULL);

  // Start application
  hostLink.go();
  std::cerr << "App started\n";

  // Message variable
  HostMsg msg;

    std::cerr << "Sending beads to thread\n";
  // Send beads to thread
  for (uint16_t x = 0; x < VOL_SIZE; x++) {
    for (uint16_t y = 0; y < VOL_SIZE; y++) {
        for (uint16_t z = 0; z < VOL_SIZE; z++) {
            unit_t t = {x, y, z};
            cell_t c = cells[t];
            uint32_t slots = c.bslot;

            while (slots) {
                msg.type = 0x0A;

                msg.from.x = x;
                msg.from.y = y;
                msg.from.z = z;

                uint32_t ci = get_next_slot(slots);

                bead_t b = c.bead_slot[ci];
                msg.beads[0].id = b.id;
                msg.beads[0].type = b.type;
                msg.beads[0].pos.x(b.pos.x());
                msg.beads[0].pos.y(b.pos.y());
                msg.beads[0].pos.z(b.pos.z());
                msg.beads[0].velo.x(0.0);
                msg.beads[0].velo.y(0.0);
                msg.beads[0].velo.z(0.0);

                slots = clear_slot(slots, ci);

                hostLink.send(0, 3, &msg);
            }
        }
    }
  }

  // Indicate end of beads being sent
  msg.type = 0xAA;
  hostLink.send(0, 3, &msg);
  std::cerr << "Sent end of transfer message\n";

    std::map<unit_t, std::map<uint32_t, bead_t>> bead_map;

    uint32_t beads_recv = 0;

    while (true) {
        // Get message from thread
        hostLink.recvMsg(&msg, sizeof(HostMsg));
        if (msg.type == 0xBB) {
          break;
        }
        beads_recv++;

        unit_t t;
        t.x = msg.from.x;
        t.y = msg.from.y;
        t.z = msg.from.z;

        uint32_t bead_id = msg.beads[0].id;

        bead_map[t][bead_id] = msg.beads[0];
    }

    for (std::map<unit_t, std::map<uint32_t, bead_t>>::iterator i = bead_map.begin(); i != bead_map.end(); ++i) {
        unit_t t = i->first;
        std::cerr << "Cell " << t.x << ", " << t.y << ", " << t.z << "\n";
        for (std::map<uint32_t, bead_t>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            bead_t b = j->second;
            std::cerr << "\tBead id   = " << b.id << ", bead type = " << b.type << "\n";
            std::cerr << "\tBead pos  = (" << b.pos.x() << ", " << b.pos.y() << ", " << b.pos.z() << ")\n";
            std::cerr << "\tBead velo = (" << b.velo.x() << ", " << b.velo.y() << ", " << b.velo.z() << ")\n";
            assert(b.pos.x() > 0 && b.pos.x() < 1 && b.pos.y() > 0 && b.pos.y() < 1 && b.pos.z() > 0 && b.pos.z() < 1);
        }
    }

    std::cerr << "Beads recv = " << beads_recv << "\n";

  // Stop timer
  gettimeofday(&finish, NULL);

  // Display time
  timersub(&finish, &start, &diff);
  double duration = (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
  printf("Time = %lf\n", duration);

  return 0;
}
