// Implementation file for the host simulation universe class

#include "universe.hpp"

#ifdef STATS
#define POLITE_DUMP_STATS
#define POLITE_COUNT_MSGS
#endif

#ifndef __UNIVERSE_IMPL
#define __UNIVERSE_IMPL
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

#if defined(OUTPUT_MAPPING) || defined(MESSAGE_COUNTER)
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

template<class S> void Universe<S>::clearLinks(FPGALinks* links) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            links->x.at(i).at(j) = 0;
            links->y.at(i).at(j) = 0;
            links->intra.at(i).at(j) = 0;
        }
    }
}

// Find the link and add an edge to it
template<class S>
void Universe<S>::followEdge(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, FPGALinks* links) {
    if (x0 == x1 && y0 == y1) {
        links->intra.at(x0).at(y0)++;
        return;
    }

    while (x0 != x1) {
        if (x0 > x1) {
            links->x.at(x0 - 1).at(y0)++;
            x0--;
        } else if (x0 < x1) {
            links->x.at(x0).at(y0)++;
            x0++;
        }
    }
    while (y0 != y1) {
        if (y0 > y1) {
            links->y.at(x0).at(y0 - 1)++;
            y0--;
        } else if (y0 < y1) {
            links->y.at(x0).at(y0)++;
            y0++;
        }
    }
}

// Find the number of edges which cross links
template<class S>
void Universe<S>::updateLinkInfo(PThreadId c_thread, unit_t c_loc, FPGALinks* links) {

    uint32_t xmask = ((1<<TinselMeshXBits)-1);
    // Get FPGA coordinates of origin cell
    uint32_t c_FPGA_y = c_thread >> (TinselLogThreadsPerBoard + TinselMeshXBits);
    uint32_t c_FPGA_x = (c_thread >> TinselLogThreadsPerBoard) & xmask;
    //Loop through all neighbours
    for (int16_t x_off = -1; x_off <= 1; x_off++) {
        for (int16_t y_off = -1; y_off <= 1; y_off++) {
            for (int16_t z_off = -1; z_off <= 1; z_off++) {
                if (x_off == 0 && y_off == 0 && z_off == 0) {
                    continue;
                }
                // Get neighbour location
                unit_t n_loc;
                n_loc.x = locOffset(c_loc.x, x_off, _D);
                n_loc.y = locOffset(c_loc.y, y_off, _D);
                n_loc.z = locOffset(c_loc.z, z_off, _D);

                PDeviceId nId = _locToId[n_loc];
                PDeviceAddr n_addr = _g->toDeviceAddr[nId];
                PThreadId n_thread = getThreadId(n_addr);
                uint32_t n_FPGA_y = n_thread >> (TinselLogThreadsPerBoard + TinselMeshXBits);
                uint32_t n_FPGA_x = (n_thread >> TinselLogThreadsPerBoard) & xmask;

                followEdge(c_FPGA_x, c_FPGA_y, n_FPGA_x, n_FPGA_y, links);
            }
        }
    }
}
#endif

#ifdef OUTPUT_MAPPING
// Output mapping info as JSON
template<class S>
void Universe<S>::outputMapping() {
    // Number of edges using the links in x and y direction for FPGAs
    clearLinks(&_link_edges);
    // JSON file
    std::string fileName = "../mapping/DPD_mapping_" + std::to_string(_D) + "_" + std::to_string(_D) + "_" + std::to_string(_D) + ".json";
    std::string output = "";
    // Open JSON
    output = "{\n";
    output += "\t\"vertices\": {\n";
    for(std::map<PDeviceId, unit_t>::iterator i = _idToLoc.begin(); i!=_idToLoc.end(); ++i) {
        PDeviceId cId = i->first;
        unit_t loc = i->second;
        std::string cellName = "\"cell_" + std::to_string(loc.x) + "_" + std::to_string(loc.y) + "_" + std::to_string(loc.z)+"\"";
        PDeviceAddr cellAddr = _g->toDeviceAddr[cId];
        PThreadId cellThread = getThreadId(cellAddr);
        output += "\t\t" + cellName +": " + std::to_string(cellThread) + ", \n";
        updateLinkInfo(cellThread, loc, &_link_edges);
    }
    // Remove trailing comma
    output = output.substr(0, output.length() - 3);
    // Close vertices section
    output += "\n\t},\n";
    // Open links section
    output += "\t\"links\": [\n";
    // For each FPGA print link information for it's x (east) and y (south) link.
    for (int i = 0; i < 6; i++) {
        output += "\t\t[ ";
        for (int j = 0; j < 8; j++) {
            uint64_t link_e = _link_edges.x.at(i).at(j);
            uint64_t link_n = _link_edges.y.at(i).at(j);
            output += "{\"x\": ";
            output += std::to_string(link_e);
            output += ", \"y\": ";
            output += std::to_string(link_n);
            output += "}, ";
        }
        output = output.substr(0, output.length() - 2);
        output += " ],\n";
    }
    output = output.substr(0, output.length() - 2);
    // Close links section
    output += "\n\t]\n";
    // Close JSON
    output += "}\n";
    // Write to file
    FILE* f = fopen(fileName.c_str(), "w+");
    fprintf(f, "%s", output.c_str());
    fclose(f);
    // File closed
    exit(0);
}
#endif

template<class S>
void Universe<S>::set_beads_added(uint32_t beads_added) {
    _beads_added = beads_added;
}

// constructor
template<class S>
Universe<S>::Universe(S size, unsigned D, uint32_t max_time) {
    _size = size;
    _D = D;
    _unit_size = _size / S(D);

#if defined(MESSAGE_COUNTER) || defined(OUTPUT_MAPPING)
    // Prep link 2D vectors
    _link_messages.x = std::vector<std::vector<uint64_t>>(6);
    _link_messages.y = std::vector<std::vector<uint64_t>>(6);
    _link_messages.intra = std::vector<std::vector<uint64_t>>(6);
    _link_edges.x = std::vector<std::vector<uint64_t>>(6);
    _link_edges.y = std::vector<std::vector<uint64_t>>(6);
    _link_edges.intra = std::vector<std::vector<uint64_t>>(6);
    for (int i = 0; i < 6; i++) {
        _link_messages.x.at(i).resize(8);
        _link_messages.y.at(i).resize(8);
        _link_messages.intra.at(i).resize(8);
        _link_edges.x.at(i).resize(8);
        _link_edges.y.at(i).resize(8);
        _link_edges.intra.at(i).resize(8);
    }
#endif

#ifdef VISUALISE
    _extern = new ExternalServer("_external.sock");
#endif

#ifdef GALS
    std::cout << "Running GALS implementation ";
  #ifdef ONE_BY_ONE
    std::cout << "using One by One";
  #endif
    std::cout << "\n";
#elif defined(ONE_BY_ONE)
    std::cout << "Running one by one version\n";
#elif defined(SEND_TO_SELF)
    std::cout << "Running send to self version\n";
#else
    std::cout << "Running standard\n";
#endif

#ifdef MESSAGE_MANAGEMENT
    std::cout << "Message management is enabled for this run\n";
#endif

#ifdef DOUBLE_SQRT
    std::cout << "Sqrt will run double number of calculations. NOTE: This will fail testing" << "\n";
#endif

    std::cout << "Test length = " << max_time << "\n";

    _boxesX = 2;//TinselBoxMeshXLen;
    _boxesY = 1;//TinselBoxMeshYLen;
    _boardsX = _boxesX * TinselMeshXLenWithinBox;
    _boardsY = _boxesY * TinselMeshYLenWithinBox;

    std::cout << "Running on " << _boxesX * _boxesY << " box";
    if ((_boxesX * _boxesY) != 1) {
        std::cout << "es";
    }
    std::cout << ".\n";

#ifndef OUTPUT_MAPPING
    _hostLink = new HostLink(_boxesX, _boxesY);
#endif
    _g = new PGraph<DPDDevice, DPDState, None, DPDMessage>(_boxesX, _boxesY);

    // create the devices
    for(uint16_t x=0; x<D; x++) {
        for(uint16_t y=0; y<D; y++) {
            for(uint16_t z=0; z<D; z++) {
                    PDeviceId id = _g->newDevice();
                    unit_t loc = {x, y, z};
                    _idToLoc[id] = loc;
                    _locToId[loc] = id;
            }
        }
    }

    // connect all the devices together appropriately
    // a toroidal space (cube with periodic boundaries)
    for(uint16_t x=0; x<_D; x++) {
        for(uint16_t y=0; y<_D; y++) {
            for(uint16_t z=0; z<_D; z++) {
                // this device
                unit_t c_loc = {x,y,z};
                PDeviceId cId = _locToId[c_loc];

                // calculate the neighbour positions
                // (taking into account the periodic boundary)
                int x_neg, y_neg, z_neg;
                int x_pos, y_pos, z_pos;

                // assign the x offsets
                if(x==0) {
                    x_neg = _D-1;
                    x_pos = x+1;
                } else if (x == (_D-1)) {
                    x_neg = x-1;
                    x_pos = 0;
                } else {
                    x_neg = x-1;
                    x_pos = x+1;
                }

                // assign the y offsets
                if(y==0) {
                    y_neg = _D-1;
                    y_pos = y+1;
                } else if (y == (_D-1)) {
                    y_neg = y-1;
                    y_pos = 0;
                } else {
                    y_neg = y-1;
                    y_pos = y+1;
                }

                // assign the z offsets
                if(z==0) {
                    z_neg = _D-1;
                    z_pos = z+1;
                } else if (z == (_D-1)) {
                    z_neg = z-1;
                    z_pos = 0;
                } else {
                    z_neg = z-1;
                    z_pos = z+1;
                }

                unit_t n_loc;
                PDeviceId nId;

            #ifdef SEND_TO_SELF
                addNeighbour(cId, cId);
            #elif defined(GALS) && !defined(ONE_BY_ONE)
                addNeighbour(cId, cId);
            #endif

                // z = -1
                // { -1,-1,-1 },  { -1,0,-1 },  { -1, +1,-1 }
                n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { 0,-1, -1 },  { 0, 0,-1 },  { 0, +1, -1 }
                n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x; n_loc.y = y; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { +1,-1,-1 },  { +1,0,-1 },  { +1, +1,-1 }
                n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_neg;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // z = 0
                // { -1,-1,0 },  { -1,0,0 },  { -1, +1,0 }
                n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { 0,-1, 0 },  { 0, 0, 0 },  { 0, +1, 0 }
                n_loc.x = x; n_loc.y = y_neg; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // skipping! one is not a neighbour of oneself
                //n_loc.x = x; n_loc.y = y; n_loc.z = z;

                n_loc.x = x; n_loc.y = y_pos; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { +1,-1, 0 },  { +1,0, 0 },  { +1, +1, 0 }
                n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // z = +1
                // { -1,-1,+1 },  { -1,0,+1},  { -1, +1,+1 }
                n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { 0,-1, +1 },  { 0, 0, +1 },  { 0, +1, +1 }
                n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x; n_loc.y = y; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                // { +1,-1, +1 },  { +1,0, +1 },  { +1, +1, +1 }
                n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);

                n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_pos;
                nId = _locToId[n_loc];
                addNeighbour(cId, nId);
            }
        }
    }
    // all the edges have been connected

#ifdef DRAM
    _g->mapVerticesToDRAM = true;
    std::cout << "Mapping vertices to DRAM\n";
#endif
    _g->map(); // map the graph into hardware calling the POLite placer

#ifdef OUTPUT_MAPPING
    outputMapping();
#endif

    // initialise all the devices with their position and the max time
    for(std::map<PDeviceId, unit_t>::iterator i = _idToLoc.begin(); i!=_idToLoc.end(); ++i) {
        PDeviceId cId = i->first;
        unit_t loc = i->second;
        _g->devices[cId]->state.loc.x = loc.x;
        _g->devices[cId]->state.loc.y = loc.y;
        _g->devices[cId]->state.loc.z = loc.z;
        _g->devices[cId]->state.unit_size = _unit_size;
        _g->devices[cId]->state.N = _D;
        _g->devices[cId]->state.max_time = max_time;
        _g->devices[cId]->state.mode = UPDATE;
        _g->devices[cId]->state.rngstate = 1234; // start with a seed
    #ifdef VISUALISE
        _g->devices[cId]->state.emitcnt = emitperiod;
    #endif
    #ifdef SMALL_DT_EARLY
        _g->devices[cId]->state.dt = early_dt;
        _g->devices[cId]->state.inv_sqrt_dt = early_inv_sqrt_dt;
    #endif
    }
}

// deconstructor
template<class S>
Universe<S>::~Universe() {
    delete _g;
}

// checks to see if a bead can be added to the universe
template<class S>
bool Universe<S>::space(const bead_t *in) {
    bead_t b = *in;
    unit_pos_t x = floor(b.pos.x()/_unit_size);
    unit_pos_t y = floor(b.pos.y()/_unit_size);
    unit_pos_t z = floor(b.pos.z()/_unit_size);
    unit_t t = {x,y,z};
    if (x > _size || x < 0 || y > _size || y < 0 || z > _size || z < 0) {
        return false;
    }

    // lookup the device
    PDeviceId b_su = _locToId[t];

    // check to make sure there is still enough room in the device
    if(get_num_beads(_g->devices[b_su]->state.bslot) >= (max_beads_per_dev)) {
        return false;
    }

    // Adjust the bead position
    b.pos.x(b.pos.x() - x);
    b.pos.y(b.pos.y() - y);
    b.pos.z(b.pos.z() - z);
    // Check to see if this bead is too close to any other beads
    if (find_nearest_bead_distance(&b, t) < 0.2) {
        return false;
    }

    return true;
}

// checks to see if a pair of beads can be added to the universe
template<class S>
bool Universe<S>::space(const bead_t *pa, const bead_t *pb) {
   unit_pos_t xa = floor(pa->pos.x()/_unit_size);
   unit_pos_t ya = floor(pa->pos.y()/_unit_size);
   unit_pos_t za = floor(pa->pos.z()/_unit_size);
   unit_t ta = {xa,ya,za};

    unit_pos_t xb = floor(pb->pos.x()/_unit_size);
   unit_pos_t yb = floor(pb->pos.y()/_unit_size);
   unit_pos_t zb = floor(pb->pos.z()/_unit_size);
   unit_t tb = {xb,yb,zb};

    if(_locToId.find(ta)==_locToId.end()){
     return false;
   }
   if(_locToId.find(tb)==_locToId.end()){
     return false;
   }

    // lookup the device
   PDeviceId b_sua = _locToId[ta];
   PDeviceId b_sub = _locToId[tb];

    if(b_sua==b_sub){
     return get_num_beads(_g->devices[b_sua]->state.bslot)+1 < max_beads_per_dev;
   }else{
     return (get_num_beads(_g->devices[b_sua]->state.bslot) < max_beads_per_dev)
           && get_num_beads(_g->devices[b_sub]->state.bslot) < max_beads_per_dev;
   }
}

// add a bead to the simulation universe
template<class S>
unit_t Universe<S>::add(const bead_t *in) {
    bead_t b = *in;
    unit_pos_t x = floor(b.pos.x()/_unit_size);
    unit_pos_t y = floor(b.pos.y()/_unit_size);
    unit_pos_t z = floor(b.pos.z()/_unit_size);
    unit_t t = {x,y,z};

    // lookup the device
    PDeviceId b_su = _locToId[t];

    // check to make sure there is still enough room in the device
    if(get_num_beads(_g->devices[b_su]->state.bslot) > max_beads_per_dev) {
        printf("Error: there is not enough space in device:%d for bead:%d  already %u beads in the slot\n", b_su, in->id, get_num_beads(_g->devices[b_su]->state.bslot));
        fflush(stdout);
        exit(EXIT_FAILURE);
    } else {
        // we can add the bead

        // make the postion of the bead relative
        b.pos.x(b.pos.x() - (S(float(t.x))*_unit_size));
        b.pos.y(b.pos.y() - (S(float(t.y))*_unit_size));
        b.pos.z(b.pos.z() - (S(float(t.z))*_unit_size));

        // get the next free slot in this device
        uint8_t slot = get_next_free_slot(_g->devices[b_su]->state.bslot);
        _g->devices[b_su]->state.bead_slot[slot] = b;
        _g->devices[b_su]->state.bslot = set_slot(_g->devices[b_su]->state.bslot, slot);
        _g->devices[b_su]->state.force_slot[slot].set(0.0, 0.0, 0.0);
    }

    return t;
}

template<class S>
void Universe<S>::add(const unit_t cell, const bead_t *in) {
    bead_t b = *in;
    if (b.pos.x() > _unit_size || b.pos.y() > _unit_size || b.pos.z() > _unit_size) {
        printf("Error: Bead position given (%f, %f, %f) is outside the bounds of this unit (%d, %d, %d) which has side length %f\n", b.pos.x(), b.pos.y(), b.pos.z(), cell.x, cell.y, cell.z, _unit_size);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // lookup the device
    PDeviceId b_su = _locToId[cell];

    // get the next free slot in this device
    uint8_t slot = get_next_free_slot(_g->devices[b_su]->state.bslot);
    _g->devices[b_su]->state.bead_slot[slot] = b;
    _g->devices[b_su]->state.bslot = set_slot(_g->devices[b_su]->state.bslot, slot);
    _g->devices[b_su]->state.force_slot[slot].set(0.0, 0.0, 0.0);
    }

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

#ifdef MESSAGE_COUNTER
template<class S>
void Universe<S>::calculateMessagesPerLink(std::map<unit_t, uint32_t> cell_messages) {
    clearLinks(&_link_messages);
    for (std::map<unit_t, uint32_t>::iterator i = cell_messages.begin(); i != cell_messages.end(); ++i) {
        clearLinks(&_link_edges);
        unit_t loc = i->first;
        uint32_t cellId = _locToId[loc];
        PDeviceAddr cellAddr = _g->toDeviceAddr[cellId];
        PThreadId cellThread = getThreadId(cellAddr);
        uint32_t messages = i->second;
        updateLinkInfo(cellThread, loc, &_link_edges);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 8; j++) {
                if (_link_edges.x.at(i).at(j) > 0) {
                    uint32_t messages_before_x = _link_messages.x.at(i).at(j);
                    _link_messages.x.at(i).at(j) += (messages * _link_edges.x.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_x <= _link_messages.x.at(i).at(j));
                }
                if (_link_edges.y.at(i).at(j) > 0) {
                    uint32_t messages_before_y = _link_messages.y.at(i).at(j);
                    _link_messages.y.at(i).at(j) += (messages * _link_edges.y.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_y <= _link_messages.y.at(i).at(j));
                }
                if (_link_edges.intra.at(i).at(j) > 0) {
                    uint32_t messages_before_intra = _link_messages.intra.at(i).at(j);
                    _link_messages.intra.at(i).at(j) += (messages * _link_edges.intra.at(i).at(j));
                    // Overflow check - if messages_before is less, we've gone back to 0
                    assert(messages_before_intra <= _link_messages.intra.at(i).at(j));
                }
            }
        }
    }

    // DEBUG: Print link message numbers to screen - link (0, 0) is bottom left corner of printed
    for (int j = 7; j >= 0; j--) {
        for (int i = 0; i < 6; i++) {
            std::cout <<_link_messages.intra.at(i).at(j) << " (" << _link_messages.x.at(i).at(j) << ", " << _link_messages.y.at(i).at(j) << "), ";
        }
        std::cout << "\n";
    }
    std::string s = "../link_messages/link_messages_" + std::to_string(_D) + ".csv";
    FILE* f = fopen(s.c_str(), "w+");

    fprintf(f, "%u\n", _D);
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            std::string intra_name = "intra (" + std::to_string(i) + " " + std::to_string(j) + ")";
            std::string x_name = "(" + std::to_string(i) + " " + std::to_string(j) + ")E - W(" + std::to_string(i+1) + " " + std::to_string(j) + ")";
            std::string y_name = "(" + std::to_string(i) + " " + std::to_string(j) + ")N - S(" + std::to_string(i) + " " + std::to_string(j+1) + ")";

            fprintf(f, "%s, %lu\n", intra_name.c_str(), _link_messages.intra.at(i).at(j));

            if (i < 5) {
                fprintf(f, "%s, %lu\n", x_name.c_str(), _link_messages.x.at(i).at(j));
            }
            if (j < 7) {
                fprintf(f, "%s, %lu\n", y_name.c_str(), _link_messages.y.at(i).at(j));
            }
        }
    }

    fclose(f);
}
#endif

// starts the simulation
template<class S>
void Universe<S>::run(uint32_t max_time) {
    _hostLink->boot("code.v", "data.v");
    _hostLink->go();

    struct timeval start, finish, elapsedTime;
    gettimeofday(&start, NULL);

#if defined(STATS)
    politeSaveStats(_hostLink, "stats.txt");
#endif

    uint32_t devices = 0;
    uint32_t timestep = 0;
#ifdef BEAD_COUNTER
    uint32_t beads_out = 0;
#endif

#ifdef MESSAGE_COUNTER
    std::map<unit_t, uint32_t> cell_messages;
#endif
    std::map<uint32_t, std::map<uint32_t, bead_t>> bead_map;
    // enter the main loop
    while(1) {
        PMessage<DPDMessage> msg;
        _hostLink->recvMsg(&msg, sizeof(msg));
    #ifdef TIMER
      #ifdef BEAD_COUNTER
        if (msg.payload.type == 0xAA) {
            devices++;
            beads_out += msg.payload.timestep;
            if (devices >= _D*_D*_D) { // All devices reported
                std::cerr << "Beads in  = " << _beads_added << "\n";
                std::cerr << "Beads out = " << beads_out << "\n";
                FILE *f = fopen("../bead_count.csv", "a+");
                fprintf(f, "%u, %u, %u\n", _D, _beads_added, beads_out);
                fclose(f);
                return;
            }
        }
      #else
        std::cerr << "Msg type = " << (uint32_t) msg.payload.type << "\n";
        if (msg.payload.type == 0xDD) {
            if (msg.payload.timestep > timestep) {
                std::cerr << msg.payload.from.x << ", "<< msg.payload.from.y << ", " << msg.payload.from.z;
                std::cerr << " finished early. Timestep " << msg.payload.timestep << "\n";
                timestep = msg.payload.timestep;
            }
        } else if (msg.payload.type != 0xBB) {
            if (msg.payload.timestep >= max_time) {
                gettimeofday(&finish, NULL);
                timersub(&finish, &start, &elapsedTime);
                double duration = (double) elapsedTime.tv_sec + (double) elapsedTime.tv_usec / 1000000.0;
                printf("Runtime = %1.10f\n", duration);
                FILE* f = fopen("../mega_results.csv", "a+");
                // FILE* f = fopen("../timing_results.csv", "a+");
                fprintf(f, "%1.10f", duration);
                fclose(f);
                return;
            } else {
                std::cerr << "ERROR: Received finish message at early timestep: " << msg.payload.timestep << "\n";
                return;
            }
        } else {
            std::cerr << "ERROR: received message when not expected\n";
            return;
        }
      #endif
    #elif defined(STATS)
        if (msg.payload.type == 0xAA) {
            printf("Stat collection complete, run \"make print-stats -C ..\"\n");
            return;
        }
    #elif defined(MESSAGE_COUNTER)
        if (msg.payload.type != 0xBB) {
            devices++;
            cell_messages[msg.payload.from] = msg.payload.timestep;
            if (devices >= (_D*_D*_D)) {
                calculateMessagesPerLink(cell_messages);
                return;
            }
        }
    #else
        if (timestep < msg.payload.timestep) {
            timestep = msg.payload.timestep;
            std::cout << "Timestep " << timestep << "\r";
        }
        // pts_to_extern_t eMsg;
        // eMsg.timestep = msg.payload.timestep;
        // eMsg.from = msg.payload.from;
        // eMsg.bead = msg.payload.beads[0];
        // _extern->send(&eMsg);
        if (msg.payload.timestep >= max_time) {
            std::cout << "\n";
            std::cout << "Finished, saving now\n";
            for (std::map<uint32_t, std::map<uint32_t, bead_t>>::iterator i = bead_map.begin(); i != bead_map.end(); ++i) {
                std::cout << "Timestep " << i->first << "\r";
                std::string path = "../25_bond_frames/state_" + std::to_string(i->first) + ".json";
                FILE* f = fopen(path.c_str(), "w+");
                fprintf(f, "{\n\t\"beads\":[\n");
                bool first = true;
                for (std::map<uint32_t, bead_t>::iterator j = i->second.begin(); j != i->second.end(); ++j){
                    if (first) {
                        first = false;
                    } else {
                        fprintf(f, ",\n");
                    }
                    fprintf(f, "\t\t{\"id\":%u, \"x\":%f, \"y\":%f, \"z\":%f, \"vx\":%f, \"vy\":%f, \"vz\":%f, \"type\":%u}", j->second.id, j->second.pos.x(), j->second.pos.y(), j->second.pos.z(), j->second.velo.x(), j->second.velo.y(), j->second.velo.z(), j->second.type);
                }
                fprintf(f, "\n]}");
                fclose(f);
            }
            std::cout << "\n";
            return;
        }
        // if (msg.payload.beads[0].id >= 0x80000000ul) {
            bead_t b = msg.payload.beads[0];
            b.pos.x(b.pos.x() + msg.payload.from.x);
            b.pos.y(b.pos.y() + msg.payload.from.y);
            b.pos.z(b.pos.z() + msg.payload.from.z);
            bead_map[msg.payload.timestep][msg.payload.beads[0].id] = b;
        // }
    #endif
    }
}

// Runs a test, gets the bead outputs and returns this to the test file
template<class S>
std::map<uint32_t, DPDMessage> Universe<S>::test() {
    std::map<uint32_t, DPDMessage> result;
    // Finish counter
    uint32_t finish = 0;
    _hostLink->boot("code.v", "data.v");
    _hostLink->go();

    // enter the main loop
    while(1) {
        PMessage<DPDMessage> msg;
        _hostLink->recvMsg(&msg, sizeof(msg));
        if (msg.payload.type == 0xAA) {
            finish++;
            if (finish >= (_D*_D*_D)) {
                return result;
            }
        } else {
            result[msg.payload.beads[0].id] = msg.payload;
        }
    }

    return result;
}

template<class S>
uint16_t Universe<S>::get_neighbour_cell_dimension(unit_pos_t c, int16_t n) {
    if (n == -1) {
        if (c == 0) {
            return _D - 1;
        } else {
            return c - 1;
        }
    } else if (n == 1) {
        if (c == _D - 1) {
            return 0;
        } else {
            return c + 1;
        }
    } else {
        return c;
    }
}

template<class S>
PDeviceId Universe<S>::get_neighbour_cell_id(unit_t u_i, int16_t d_x, int16_t d_y, int16_t d_z) {
    unit_t u_j = {
        get_neighbour_cell_dimension(u_i.x, d_x),
        get_neighbour_cell_dimension(u_i.y, d_y),
        get_neighbour_cell_dimension(u_i.z, d_z)
    };
    return _locToId[u_j];
}

template<class S>
float Universe<S>::find_nearest_bead_distance(const bead_t *i, unit_t u_i) {
    float min_dist = 100.0;
    for (int16_t d_x = -1; d_x <= 1; d_x++) {
        for (int16_t d_y = -1; d_y <= 1; d_y++) {
            for (int16_t d_z = -1; d_z <= 1; d_z++) {
                PDeviceId n_id = get_neighbour_cell_id(u_i, d_x, d_y, d_z);
                // Get neighbour bead slot
                uint32_t nslot = _g->devices[n_id]->state.bslot;
                while (nslot) {
                    uint32_t cj = get_next_slot(nslot);
                    nslot = clear_slot(nslot, cj);
                    bead_t j = _g->devices[n_id]->state.bead_slot[cj];
                    if (j.id == i->id) {
                        continue;
                    }
                    Vector3D<float> j_adj; // Adjust the neighbour bead, j, relative to the given bead, i
                    j_adj.x(j.pos.x() + d_x);
                    j_adj.y(j.pos.y() + d_y);
                    j_adj.z(j.pos.z() + d_z);
                    // Get euclidean distance and store it if its smaller than the current min
                    float dist = j_adj.dist(i->pos);
                    if (dist < min_dist) {
                        min_dist = dist;
                    }
                }
            }
        }
    }
    return min_dist;
}

template<class S>
void Universe<S>::store_initial_bead_distances() {
    std::cerr << "Outputting minimum distances between beads for initial placement to ../init_dist.json\n";
    FILE* f = fopen("../init_dist.json", "w+");
    fprintf(f, "{ \"min_dists\":[\n");
    bool first = true;
    for (unit_pos_t u_x = 0; u_x < _D; u_x++) {
        for (unit_pos_t u_y = 0; u_y < _D; u_y++) {
            for (unit_pos_t u_z = 0; u_z < _D; u_z++) {
                unit_t u = { u_x, u_y, u_z };
                PDeviceId dev_id = _locToId[u];
                uint32_t bslot = _g->devices[dev_id]->state.bslot;
                while (bslot) {
                    uint32_t i = get_next_slot(bslot);
                    bead_t b = _g->devices[dev_id]->state.bead_slot[i];
                    if (first) {
                        first = false;
                    } else {
                        fprintf(f, ",\n");
                    }
                    fprintf(f, "\t %f", find_nearest_bead_distance(&b, u));
                    bslot = clear_slot(bslot, i);
                }
            }
        }
    }
    fprintf(f, "\n]}");
    fclose(f);
    std::cerr << "Complete\n";
}

#endif /* __UNIVERSE_IMPL */
