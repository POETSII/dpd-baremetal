// Implementation file for the host simulation universe class

#include "parseUniverse.hpp"

#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS

#ifndef __PARSE_UNIVERSE_IMPL
#define __PARSE_UNIVERSE_IMPL
#include <boost/algorithm/string.hpp>
#include <iomanip>

// helper functions for managing bead slots
template<class S>
uint8_t Universe<S>::clear_slot(uint8_t slotlist, uint8_t pos){  return slotlist & ~(1 << pos);  }
template<class S>
uint8_t Universe<S>::set_slot(uint8_t slotlist, uint8_t pos){ return slotlist | (1 << pos); }
template<class S>
bool Universe<S>::is_slot_set(uint8_t slotlist, uint8_t pos){ return slotlist & (1 << pos); }

// print out the occupancy of each device
template<class S>
void Universe<S>::print_occupancy() {
    // loop through all devices in the universe and print their number of particles assigned
    printf("DeviceId\t\tbeads\n--------------\n");
    for(auto const& x : _idToLoc) {
        PDeviceId t = x.first;
        uint8_t beads = get_num_beads(_g->devices[t]->state.bslot);
        if(beads > 0)
            printf("%x\t\t\t%d\n", t, (uint32_t)beads);
    }
}

template<class S>
uint8_t Universe<S>::get_next_slot(uint8_t slotlist) {
    uint8_t mask = 0x1;
    for(int i=0; i<8; i++) {
        if(slotlist & mask) {
            return i;
        }
        mask = mask << 1; // shift to the next pos
    }
    return 0xF; // we are empty
}

template<class S>
uint8_t Universe<S>::get_next_free_slot(uint8_t slotlist) {
    uint8_t mask = 0x1;
    for(int i=0; i<8; i++){
        if(!(slotlist & mask)) {
           return i;
        }
        mask = mask << 1;
    }
    return 0xF; // error there are no free slots!
}

template<class S>
void Universe<S>::print_slot(uint8_t slotlist) {
    printf("slotlist = ");
    uint8_t mask = 0x1;
    for(int i=0; i<8; i++) {
        if(slotlist & mask) {
            printf("1");
        } else {
            printf("0");
        }
        mask = mask << 1;
    }
    printf("\n");
}

// get the number of beads occupying a slot
template<class S>
uint8_t Universe<S>::get_num_beads(uint8_t slotlist) {
    uint8_t cnt = 0;
    uint8_t mask = 0x1;
    for(int i=0; i<8; i++) {
        if(slotlist & mask) {
            cnt++;
        }
        mask = mask << 1;
    }
    return cnt; // error there are no free slots!
}

// make two devices neighbours
template<class S>
void Universe<S>::addNeighbour(PDeviceId a, PDeviceId b) {
    _g->addEdge(a,0,b);
    //_g->addEdge(b,0,a);
}

template<class S>
void Universe<S>::createDevices() {
    int64_t numDevices = 0;
    for(uint16_t x = 0; x < _volume.x(); x++) {
        for(uint16_t y = 0; y < _volume.y(); y++) {
            for(uint16_t z = 0; z < _volume.z(); z++) {
                    PDeviceId id = _g->newDevice();
                    unit_t loc = {x, y, z};
                    _idToLoc[id] = loc;
                    _locToId[loc] = id;
                    numDevices++;
            }
        }
    }
    std::cout << "All cells added to universe.   Total cells: " << numDevices << "\n";
}

template<class S>
uint16_t Universe<S>::locOffset(const uint16_t current, const int16_t offset, const float vol_max) {
    if (offset == -1 && current == 0) {
        return vol_max - 1;
    } else if (offset == 1 && current == vol_max - 1) {
        return 0;
    } else {
        return current + offset;
    }
}

template<class S>
void Universe<S>::connectDevices() {
    int64_t numEdges = 0;
    for (uint16_t x = 0; x < _volume.x(); x++) {
        for (uint16_t y = 0; y < _volume.y(); y++) {
            for (uint16_t z = 0; z < _volume.z(); z++) {
                // This device
                unit_t c_loc = {x, y, z};
                PDeviceId cId = _locToId[c_loc];

                for (int16_t x_off = -1; x_off <= 1; x_off++) {
                    for (int16_t y_off = -1; y_off <= 1; y_off++) {
                        for (int16_t z_off = -1; z_off <= 1; z_off++) {
                            if (x_off == 0 && y_off == 0 && z_off == 0)
                                continue;

                            // Get neighbour location
                            unit_t n_loc;
                            n_loc.x = locOffset(c_loc.x, x_off, _volume.x());
                            n_loc.y = locOffset(c_loc.y, y_off, _volume.y());
                            n_loc.z = locOffset(c_loc.z, z_off, _volume.z());
                            assert (n_loc.x >= 0 && n_loc.x < _volume.x() && n_loc.y >= 0 && n_loc.y < _volume.y() && n_loc.z >= 0 && n_loc.z < _volume.z());

                            PDeviceId nId = _locToId[n_loc];
                            addNeighbour(cId, nId);
                            numEdges++;
                        }
                    }
                }
            }
        }
    }
    std::cout << "All cells have been connected. Total edges: " << numEdges << "\n";
}

template<class S>
void Universe<S>::initialiseCells(DPDSimulation sim) {
    for(std::map<PDeviceId, unit_t>::iterator i = _idToLoc.begin(); i!=_idToLoc.end(); ++i) {
        // Device id
        PDeviceId cId = i->first;
        // Set cell location
        unit_t loc = i->second;
        // Set cell dimensions
        _g->devices[cId]->state.unit_dimensions = sim.getCell();
        // Set volume so this cell knows how large the entire universe is
        _g->devices[cId]->state.volume_dimensions = sim.getVolume();
        // Set the radii of all bead types in the simulation
        for (std::map<bead_type_id, Bead_type>::iterator i = sim.getBeadTypes().begin(); i != sim.getBeadTypes().end(); ++i) {
            _g->devices[cId]->state.r_c[i->second.type] = i->second.radius;
        }
        // Set the conservative force for all possible bead types
        int i_pos = 0;
        std::vector<std::vector<float>> conservativeParameters = sim.getConservativeParameters();
        for (std::vector<std::vector<float>>::iterator i = conservativeParameters.begin(); i != conservativeParameters.end(); ++i) {
            int j_pos = 0;
            for (std::vector<float>::iterator j = i->begin(); j != i->end(); ++j) {
                _g->devices[cId]->state.a[i_pos][j_pos] = *j;
                j_pos++;
            }
            i_pos++;
        }
        std::cout << "[\n";
        for(int i = 0; i < 5; i++) {
            std::cout << "[ ";
            for (int j = 0; j < 5; j++) {
                std::cout << _g->devices[cId]->state.a[i][j];
                if (j < 4)
                    std::cout << ", ";
            }
            std::cout << "]\n";
        }
        std::cout << "]\n";
        // _g->devices[cId]->state.loc.x = loc.x;
        // _g->devices[cId]->state.loc.y = loc.y;
        // _g->devices[cId]->state.loc.z = loc.z;
        // _g->devices[cId]->state.unit_size = _unit_size;
        // _g->devices[cId]->state.N = _D;
    }
}

// constructor
template<class S>
Universe<S>::Universe(DPDSimulation sim) {
    if (sim.getTitle() != "") {
        std::cout << "Simulation title: " << sim.getTitle() << "\n";
    }

    _volume = sim.getVolume();
    std::cout << "Volume dimensions: " << _volume.x() << ", " << _volume.y() << ", " << _volume.z() << "\n";

#ifdef VISUALISE
    _extern = new ExternalServer("_external.sock");
#endif
    _boxesX = TinselBoxMeshXLen;
    _boxesY = TinselBoxMeshYLen;
    _hostLink = new HostLink(_boxesX, _boxesY);
    _g = new PGraph<DPDDevice, DPDState, None, DPDMessage>(_boxesX, _boxesY);

    // Create the devices
    createDevices();

    // Connect each device to it's neighbours - Each has 26
    connectDevices();

    // Optional - Mapping vertices to DRAM allows for more devices, at the cost of slower to read from DRAM.
    // _g->mapVerticesToDRAM = true;
#ifndef TIMER
    // Map the graph into hardware calling the POLite placer
    _g->map();
#else
    // Map the graph into hardware, but add "Timer" devices to the mapping also, which synchronise TinselCycleCount across all available boards
    // This allows for more accurate timing, from when the first device starts to the first device finishes.
    // This mapper copies PGraph's map(), but adds timers and maps these separately.
    timerMap(_g, TinselBoxMeshXLen, TinselBoxMeshYLen); // 4 POETS Boxes
#endif

    // Initialise all the devices with necessary data to run a simulation
    initialiseCells(sim);
//     // initialise all the devices with their position
//     for(std::map<PDeviceId, unit_t>::iterator i = _idToLoc.begin(); i!=_idToLoc.end(); ++i) {
//         PDeviceId cId = i->first;
//         unit_t loc = i->second;
//         _g->devices[cId]->state.loc.x = loc.x;
//         _g->devices[cId]->state.loc.y = loc.y;
//         _g->devices[cId]->state.loc.z = loc.z;
//         _g->devices[cId]->state.unit_size = _unit_size;
//         _g->devices[cId]->state.N = _D;
//     }
}

// deconstructor
template<class S>
Universe<S>::~Universe() {
    delete _g;
}

// checks to see if a bead can be added to the universe
// template<class S>
// bool Universe<S>::space(const bead_t *in) {
//     bead_t b = *in;
//     unit_pos_t x = floor(b.pos.x()/_unit_size);
//     unit_pos_t y = floor(b.pos.y()/_unit_size);
//     unit_pos_t z = floor(b.pos.z()/_unit_size);
//     unit_t t = {x,y,z};
//     if (x > _size || x < 0 || y > _size || y < 0 || z > _size || z < 0) {
//         return false;
//     }

//     // lookup the device
//     PDeviceId b_su = _locToId[t];

//     // check to make sure there is still enough room in the device
//     if(get_num_beads(_g->devices[b_su]->state.bslot) >= (max_beads_per_dev)) {
//         return false;
//     } else {
//         return true;
//     }
// }

// checks to see if a pair of beads can be added to the universe
// template<class S>
// bool Universe<S>::space(const bead_t *pa, const bead_t *pb) {
//    unit_pos_t xa = floor(pa->pos.x()/_unit_size);
//    unit_pos_t ya = floor(pa->pos.y()/_unit_size);
//    unit_pos_t za = floor(pa->pos.z()/_unit_size);
//    unit_t ta = {xa,ya,za};

//     unit_pos_t xb = floor(pb->pos.x()/_unit_size);
//    unit_pos_t yb = floor(pb->pos.y()/_unit_size);
//    unit_pos_t zb = floor(pb->pos.z()/_unit_size);
//    unit_t tb = {xb,yb,zb};

//     if(_locToId.find(ta)==_locToId.end()){
//      return false;
//    }
//    if(_locToId.find(tb)==_locToId.end()){
//      return false;
//    }

//     // lookup the device
//    PDeviceId b_sua = _locToId[ta];
//    PDeviceId b_sub = _locToId[tb];

//     if(b_sua==b_sub){
//      return get_num_beads(_g->devices[b_sua]->state.bslot)+1 < max_beads_per_dev;
//    }else{
//      return (get_num_beads(_g->devices[b_sua]->state.bslot) < max_beads_per_dev)
//            && get_num_beads(_g->devices[b_sub]->state.bslot) < max_beads_per_dev;
//    }
// }

// add a bead to the simulation universe
// template<class S>
// unit_t Universe<S>::add(const bead_t *in) {
//     bead_t b = *in;
//     unit_pos_t x = floor(b.pos.x()/_unit_size);
//     unit_pos_t y = floor(b.pos.y()/_unit_size);
//     unit_pos_t z = floor(b.pos.z()/_unit_size);
//     unit_t t = {x,y,z};

//     // lookup the device
//     PDeviceId b_su = _locToId[t];

//     // check to make sure there is still enough room in the device
//     if(get_num_beads(_g->devices[b_su]->state.bslot) > max_beads_per_dev) {
//         printf("Error: there is not enough space in device:%d for bead:%d  already %u beads in the slot\n", b_su, in->id, get_num_beads(_g->devices[b_su]->state.bslot));
//         fflush(stdout);
//         exit(EXIT_FAILURE);
//     } else {
//         // we can add the bead

//         // make the postion of the bead relative
//         b.pos.x(b.pos.x() - (S(float(t.x))*_unit_size));
//         b.pos.y(b.pos.y() - (S(float(t.y))*_unit_size));
//         b.pos.z(b.pos.z() - (S(float(t.z))*_unit_size));

//         // get the next free slot in this device
//         uint8_t slot = get_next_free_slot(_g->devices[b_su]->state.bslot);
//         _g->devices[b_su]->state.bead_slot[slot] = b;
//         _g->devices[b_su]->state.bslot = set_slot(_g->devices[b_su]->state.bslot, slot);
//     }

//     return t;
// }

// template<class S>
// void Universe<S>::add(const unit_t cell, const bead_t *in) {
//     bead_t b = *in;
//     if (b.pos.x() > _unit_size || b.pos.y() > _unit_size || b.pos.z() > _unit_size) {
//         printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this unit (%d, %d, %d) which has side length %f\n", b.pos.x(), b.pos.y(), b.pos.z(), cell.x, cell.y, cell.z, _unit_size);
//         fflush(stdout);
//         exit(EXIT_FAILURE);
//     }

//     // lookup the device
//     PDeviceId b_su = _locToId[cell];

//     // get the next free slot in this device
//     uint8_t slot = get_next_free_slot(_g->devices[b_su]->state.bslot);
//     _g->devices[b_su]->state.bead_slot[slot] = b;
//     _g->devices[b_su]->state.bslot = set_slot(_g->devices[b_su]->state.bslot, slot);
// }

// writes the universe into the POETS system
template<class S>
void Universe<S>::write() {
    _g->write(_hostLink);
}

// Use unit_t location to acquire thread id
template<class S>
PThreadId Universe<S>::get_thread_from_loc(unit_t loc) {
    PDeviceId dev_id = _locToId[loc];
    PDeviceAddr dev_addr = _g->toDeviceAddr[dev_id];
    PThreadId thread_id = getThreadId(dev_addr);
    return thread_id;
}

// starts the simulation
template<class S>
void Universe<S>::run(bool printBeadNum, uint32_t beadNum) {
    _hostLink->boot("parsedCode.v", "parsedData.v");
    gettimeofday(&_start, NULL);
    _hostLink->go();

#ifdef TIMER
    uint32_t devices = 0;
    uint32_t timers = 0;
    std::map<uint32_t,uint64_t> board_start;
    std::map<uint32_t,uint32_t> board_wrap;
    std::map<unit_t, uint64_t> dpd_start;
    std::map<unit_t, uint64_t> dpd_end;
    std::map<unit_t, uint32_t> locToThread;
    uint64_t earliest_start = 0xFFFFFFFFFFFFFFFF;
    uint64_t earliest_end = 0xFFFFFFFFFFFFFFFF;
#elif defined(STATS)
    uint32_t stats_finished = 0;
    uint32_t lost_beads = 0;
    uint32_t migrations = 0;
#endif

    // enter the main loop
    while(1) {
        PMessage<None, DPDMessage> msg;
        _hostLink->recvMsg(&msg, sizeof(msg));
    #ifdef TIMER
        if (msg.payload.type == 0xAB) {
            timers++;
            uint64_t t = (uint64_t) msg.payload.timestep << 32 | msg.payload.extra;
            unit_t timer_loc;
            timer_loc.x = msg.payload.from.x;
            timer_loc.y = msg.payload.from.y;
            timer_loc.z = msg.payload.from.z;
            PThreadId timer_thread = get_thread_from_loc(timer_loc);
            board_start[(uint32_t)timer_thread/1024] = t;
        } else if (msg.payload.type == 0xAA || msg.payload.type == 0xAC) {
            devices++;
            unit_t cell_loc;
            cell_loc.x = msg.payload.from.x;
            cell_loc.y = msg.payload.from.y;
            cell_loc.z = msg.payload.from.z;
            uint32_t wraps = (uint32_t) msg.payload.beads[0].pos.x();
            uint32_t thread = get_thread_from_loc(cell_loc);
            board_wrap[thread/1024] = wraps;
            uint64_t s = (uint64_t) msg.payload.timestep << 32 | msg.payload.extra;
            uint64_t e = (uint64_t) msg.payload.beads[0].id << 32 | msg.payload.beads[0].type;
            PThreadId threadId = get_thread_from_loc(cell_loc);
            dpd_start[cell_loc] = s;
            dpd_end[cell_loc] = e;
            locToThread[cell_loc] = threadId;
        }
        if (devices >= (_D*_D*_D) && timers >= 6) {
            for(std::map<unit_t, uint64_t>::iterator i = dpd_start.begin(); i!=dpd_start.end(); ++i) {
                uint32_t threadId = locToThread[i->first];
                uint32_t board = (uint32_t) threadId/1024;
                uint64_t s = dpd_start[i->first] - board_start[board];
                uint64_t e = (uint64_t) board_wrap[board] << 40 | dpd_end[i->first];
                e = e - board_start[board];
                if (s < earliest_start) {
                    earliest_start = s;
                }
                if (e < earliest_end) {
                    earliest_end = e;
                }
            }
            uint64_t diff = earliest_end - earliest_start;
            double time = (double)diff/250000000;
            printf("Runtime = %f\n", time);
            FILE* f = fopen("../timing_results.csv", "a+");
            if (printBeadNum) {
                fprintf(f, "%u, %u, ", _D, beadNum);
            }
            fprintf(f, "%1.10f", time);
            fclose(f);
            return;
        }
    #elif defined(STATS)
        if (msg.payload.type = 0xAA) {
            stats_finished++;
            lost_beads += msg.payload.timestep;
            migrations += msg.payload.beads[0].id;
            if (stats_finished >= _D*_D*_D) {
                politeSaveStats(_hostLink, "stats.txt");
                printf("Lost beads = %u\n", lost_beads);
                printf("migrations = %u\n", migrations);
                printf("Stat collection complete, run \"make print-stats -C ..\"\n");
                return;
            }
        }
    #elif defined(VISUALISE)
        pts_to_extern_t eMsg;
        eMsg.timestep = msg.payload.timestep;
        eMsg.from = msg.payload.from;
        eMsg.bead = msg.payload.beads[0];
        _extern->send(&eMsg);
    #endif
    }
}

// Runs a test, gets the bead outputs and returns this to the test file
// template<class S>
// std::map<uint32_t, DPDMessage> Universe<S>::test() {
//     std::map<uint32_t, DPDMessage> result;
//     // Finish counter
//     uint32_t finish = 0;
//     _hostLink->boot("code.v", "data.v");
//     _hostLink->go();

//     // enter the main loop
//     while(1) {
//         PMessage<None, DPDMessage> msg;
//         _hostLink->recvMsg(&msg, sizeof(msg));
//        if (msg.payload.type == 0xAA) {
//             finish++;
//             if (finish >= (_D*_D*_D)) {
//                 return result;
//             }
//         } else {
//            result[msg.payload.beads[0].id] = msg.payload;
//         }
//    }

//     return result;
// }

#endif /* __PARSE_UNIVERSE_IMPL */
