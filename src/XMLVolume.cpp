// Implementation file for the host SimVolume class

#include "XMLVolume.hpp"

#ifdef STATS
#define XML_DUMP_STATS
#define XML_COUNT_MSGS
#endif

#ifndef __XMLVOLUME_IMPL
#define __XMLVOLUME_IMPL

// // Make two devices neighbours
// template<class S>
// void SimVolume<S>::addNeighbour(PDeviceId a, PDeviceId b) {
// #ifdef SERIAL
//     DPDState *a_state = &this->cells.at(a);
//     a_state->neighbours[a_state->num_neighbours++] = b;
// #else
//     this->cells->addEdge(a,0,b);
// #endif
// }

// constructor
template<class S>
XMLVolume<S>::XMLVolume(S volume_length, unsigned cells_per_dimension) : SimVolume<S, std::vector<DPDState>>(volume_length, cells_per_dimension) {
    this->cells = std::vector<DPDState>();
}

// #ifdef GALS
//     std::cout << "Building a GALS volume.\n";
// #elif defined(SERIAL)
//     std::cout << "Building a serial volume.\n";
//     std::cout << "The DPD algorithm will be run on this x86 machine in serial.\n";
// #else
//     std::cout << "Building a synchronous volume.\n";
// #endif

// #if defined(ONE_BY_ONE)
//     std::cout << "Local bead interactions will be performed one at a time (OneByOne).\n";
// #elif defined(SEND_TO_SELF)
//     std::cout << "Cells will send beads to themselves to calculate bead interactions (SendToSelf).\n";
// #endif

// #ifdef MESSAGE_MANAGEMENT
//     std::cout << "Messages are sent at intervals determined by the number of messages sent by neighbours (MessageManagement).\n";
// #endif

// #ifdef DOUBLE_SQRT
//     std::cout << "Sqrt will run double number of calculations. NOTE: This will fail testing\n";
// #endif

//     // Connect the devices together appropriately.
//     // A toroidal space (cube with periodic boundaries)
//     for(uint16_t x = 0; x < cells_per_dimension; x++) {
//         for(uint16_t y = 0; y < cells_per_dimension; y++) {
//             for(uint16_t z = 0; z < cells_per_dimension; z++) {
//                 // This device
//                 cell_t c_loc = {x,y,z};
//                 PDeviceId cId = this->locToId[c_loc];

//                 // Calculate the neighbour positions
//                 // (taking into account the periodic boundary).
//                 int x_neg, y_neg, z_neg;
//                 int x_pos, y_pos, z_pos;

//                 // assign the x offsets
//                 if (x == 0) {
//                     x_neg = cells_per_dimension - 1;
//                     x_pos = x + 1;
//                 } else if (x == (cells_per_dimension - 1)) {
//                     x_neg = x - 1;
//                     x_pos = 0;
//                 } else {
//                     x_neg = x - 1;
//                     x_pos = x + 1;
//                 }

//                 // assign the y offsets
//                 if(y == 0) {
//                     y_neg = cells_per_dimension - 1;
//                     y_pos = y + 1;
//                 } else if (y == (cells_per_dimension - 1)) {
//                     y_neg = y - 1;
//                     y_pos = 0;
//                 } else {
//                     y_neg = y - 1;
//                     y_pos = y + 1;
//                 }

//                 // assign the z offsets
//                 if(z == 0) {
//                     z_neg = cells_per_dimension - 1;
//                     z_pos = z + 1;
//                 } else if (z == (cells_per_dimension - 1)) {
//                     z_neg = z - 1;
//                     z_pos = 0;
//                 } else {
//                     z_neg = z - 1;
//                     z_pos = z + 1;
//                 }

//                 // Neighbour location and ID
//                 cell_t n_loc;
//                 PDeviceId nId;

//                 // If we use send to self, a cell needs to be connected to itself
//             #ifdef SEND_TO_SELF
//                 addNeighbour(cId, cId);
//             #elif defined(GALS) && !defined(ONE_BY_ONE)
//                 // Non-improved GALS uses send to self by default
//                 addNeighbour(cId, cId);
//             #endif

//                 // z = -1
//                 // { -1,-1,-1 },  { -1,0,-1 },  { -1, +1,-1 }
//                 n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { 0,-1, -1 },  { 0, 0,-1 },  { 0, +1, -1 }
//                 n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x; n_loc.y = y; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { +1,-1,-1 },  { +1,0,-1 },  { +1, +1,-1 }
//                 n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_neg;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // z = 0
//                 // { -1,-1,0 },  { -1,0,0 },  { -1, +1,0 }
//                 n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { 0,-1, 0 },  { 0, 0, 0 },  { 0, +1, 0 }
//                 n_loc.x = x; n_loc.y = y_neg; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // skipping! one is not a neighbour of oneself
//                 //n_loc.x = x; n_loc.y = y; n_loc.z = z;

//                 n_loc.x = x; n_loc.y = y_pos; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { +1,-1, 0 },  { +1,0, 0 },  { +1, +1, 0 }
//                 n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // z = +1
//                 // { -1,-1,+1 },  { -1,0,+1},  { -1, +1,+1 }
//                 n_loc.x = x_neg; n_loc.y = y_neg; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_neg; n_loc.y = y_pos; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { 0,-1, +1 },  { 0, 0, +1 },  { 0, +1, +1 }
//                 n_loc.x = x; n_loc.y = y_neg; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x; n_loc.y = y; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x; n_loc.y = y_pos; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 // { +1,-1, +1 },  { +1,0, +1 },  { +1, +1, +1 }
//                 n_loc.x = x_pos; n_loc.y = y_neg; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);

//                 n_loc.x = x_pos; n_loc.y = y_pos; n_loc.z = z_pos;
//                 nId = this->locToId[n_loc];
//                 addNeighbour(cId, nId);
//             }
//         }
//     }
//     // All the edges have been connected

//     // Initialise all the devices with as much as possible to reduce code in the cell
//     init_cells();
// }

// // deconstructor
// template<class S>
// SimVolume<S>::~SimVolume() {
//     Volume<S, PGraph<DPDDevice, DPDState, None, DPDMessage> *>::~Volume();
// }

// template<class S>
// void SimVolume<S>::init_cells() {

// #if !defined(SERIAL) && !defined(RDF)
//   #ifdef DRAM
//     // Larger runs will need cells mapped to DRAM instead of SRAM
//     this->cells->mapVerticesToDRAM = true;
//     std::cout << "Mapping vertices to DRAM\n";
//   #endif
//     // Map to the hardware
//     this->cells->map();
// #endif

//     // Place all cell locations in its state
//     for (std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
//         PDeviceId id = i->first;
//         cell_t loc = i->second;
//       #if defined(SERIAL) || defined(RDF)
//         DPDState *state = cells.at(id);
//       #else
//         DPDState *state = &this->cells->devices[id]->state;
//       #endif
//         state->loc.x = loc.x;
//         state->loc.y = loc.y;
//         state->loc.z = loc.z;
//     }

//     // Add Simulation data
//     for(std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
//         PDeviceId cId = i->first;
//         // Get cell state
//       #ifdef SERIAL
//         DPDState *state = cells.at(cId);
//       #else
//         DPDState *state = &this->cells->devices[cId]->state;
//       #endif

//         // Location is set by Volume parent class
//     #ifndef SERIAL
//         // Set the volume information in the cell
//         state->cell_length = this->cell_length;
//         state->cells_per_dimension = this->cells_per_dimension;
//         state->mode = UPDATE;
//     #endif
//         state->rngstate = 1234; // start with a seed
//     #if defined(VISUALISE) && !defined(SERIAL)
//         state->emitcnt = 1; // Don't emit until emitperiod has been reached
//     #endif
//     #ifndef SERIAL
//         state->error = 0; // Error returned if something goes wrong (!= 0)
//     #endif
//         for (int s = 0; s < MAX_BEADS; s++) {
//             state->force_slot[s].set(0.0, 0.0, 0.0);
//         #ifdef BETTER_VERLET
//             state->old_velo[s].set(0.0, 0.0, 0.0);
//         #endif
//         }
//     }
// }

// // Checks to see if a bead can be added to the volume
// template<class S>
// bool SimVolume<S>::space_for_bead(const bead_t *in) {
//     bead_t b = *in;
//     cell_pos_t x = floor(b.pos.x()/this->cell_length);
//     cell_pos_t y = floor(b.pos.y()/this->cell_length);
//     cell_pos_t z = floor(b.pos.z()/this->cell_length);

//     cell_t t = {x,y,z};
//     if (x >= this->volume_length || x < 0 || y >= this->volume_length || y < 0 || z >= this->volume_length || z < 0) {
//         return false;
//     }

//     // Find the device
//     PDeviceId b_su = this->locToId[t];
//     // Get the device
//   #ifdef SERIAL
//     uint32_t bslot = cells.at(b_su)->bslot;
//   #else
//     uint32_t bslot = this->cells->devices[b_su]->state.bslot;
//   #endif

//     // Check to make sure there is still enough room in the device
//     if(get_num_beads(bslot) >= MAX_BEADS) {
//         return false;
//     }

//     // Adjust the bead position
//     b.pos.x(b.pos.x() - x);
//     b.pos.y(b.pos.y() - y);
//     b.pos.z(b.pos.z() - z);

//     if (find_nearest_bead_distance(&b, t) < 0.45) {
//         return false;
//     }

//     return true;
// }

// // Checks to see if a pair of beads can be added to the volume
// template<class S>
// bool SimVolume<S>::space_for_bead_pair(const bead_t *pa, const bead_t *pb) {
//     cell_pos_t xa = floor(pa->pos.x() / this->cell_length);
//     cell_pos_t ya = floor(pa->pos.y() / this->cell_length);
//     cell_pos_t za = floor(pa->pos.z() / this->cell_length);
//     cell_t ta = {xa,ya,za};

//     cell_pos_t xb = floor(pb->pos.x() / this->cell_length);
//     cell_pos_t yb = floor(pb->pos.y() / this->cell_length);
//     cell_pos_t zb = floor(pb->pos.z() / this->cell_length);
//     cell_t tb = {xb,yb,zb};

//     if (this->locToId.find(ta) == this->locToId.end()){
//         return false;
//     }

//     if (this->locToId.find(tb) == this->locToId.end()){
//         return false;
//     }

//     // lookup the device
//    PDeviceId b_sua = this->locToId[ta];
//    PDeviceId b_sub = this->locToId[tb];

//     if (b_sua == b_sub) {
//       #ifdef SERIAL
//         return get_num_beads(this->cells.at(b_sua)->bslot) + 1 < MAX_BEADS;
//       #else
//         return get_num_beads(this->cells.devices[b_sua]->state.bslot) + 1 < MAX_BEADS;
//       #endif
//     } else {
//       #ifdef SERIAL
//         return (get_num_beads(this->cells.at(b_sua)->bslot) < MAX_BEADS) && (get_num_beads(this->cells.getCell(b_sub)->bslot) < MAX_BEADS);
//       #else
//         return (get_num_beads(this->cells.devices[b_sua]->state.bslot) < MAX_BEADS) && (get_num_beads(this->cells.devices[b_sub]->state.bslot) < MAX_BEADS);
//       #endif
//    }
// }

// template<class S>
// uint16_t SimVolume<S>::get_neighbour_cell_dimension(cell_pos_t c, int16_t n) {
//     if (n == -1) {
//         if (c == 0) {
//             return this->cells_per_dimension - 1;
//         } else {
//             return c - 1;
//         }
//     } else if (n == 1) {
//         if (c == this->cells_per_dimension - 1) {
//             return 0;
//         } else {
//             return c + 1;
//         }
//     } else {
//         return c;
//     }
// }

// template<class S>
// PDeviceId SimVolume<S>::get_neighbour_cell_id(cell_t u_i, int16_t d_x, int16_t d_y, int16_t d_z) {
//     cell_t u_j = {
//         get_neighbour_cell_dimension(u_i.x, d_x),
//         get_neighbour_cell_dimension(u_i.y, d_y),
//         get_neighbour_cell_dimension(u_i.z, d_z)
//     };
//     return this->locToId[u_j];
// }

// template<class S>
// float SimVolume<S>::find_nearest_bead_distance(const bead_t *i, cell_t u_i) {
//     float min_dist = 100.0;
//     for (int16_t d_x = -1; d_x <= 1; d_x++) {
//         for (int16_t d_y = -1; d_y <= 1; d_y++) {
//             for (int16_t d_z = -1; d_z <= 1; d_z++) {
//                 PDeviceId n_id = get_neighbour_cell_id(u_i, d_x, d_y, d_z);
//               #ifdef SERIAL
//                 DPDState *state = this->cells.getCell(n_id);
//               #else
//                 DPDState *state = &this->cells->devices[n_id]->state;
//               #endif
//                 // Get neighbour bead slot
//                 uint32_t nslot = state->bslot;
//                 while (nslot) {
//                     uint8_t cj = get_next_slot(nslot);
//                     nslot = clear_slot(nslot, cj);
//                     bead_t j = state->bead_slot[cj];
//                     if (j.id == i->id) {
//                         continue;
//                     }
//                     Vector3D<float> j_adj; // Adjust the neighbour bead, j, relative to the given bead, i
//                     j_adj.x(j.pos.x() + d_x);
//                     j_adj.y(j.pos.y() + d_y);
//                     j_adj.z(j.pos.z() + d_z);
//                     // Get euclidean distance and store it if its smaller than the current min
//                     float dist = j_adj.dist(i->pos);
//                     if (dist < min_dist) {
//                         min_dist = dist;
//                     }
//                 }
//             }
//         }
//     }
//     return min_dist;
// }

// template<class S>
// void SimVolume<S>::store_initial_bead_distances() {
//     std::cerr << "Outputting minimum distances between beads for initial placement to ../init_dist.json\n";
//     FILE* f = fopen("../init_dist.json", "w+");
//     fprintf(f, "{ \"min_dists\":[\n");
//     bool first = true;
//     for (cell_pos_t u_x = 0; u_x < this->cells_per_dimension; u_x++) {
//         for (cell_pos_t u_y = 0; u_y < this->cells_per_dimension; u_y++) {
//             for (cell_pos_t u_z = 0; u_z < this->cells_per_dimension; u_z++) {
//                 cell_t u = { u_x, u_y, u_z };
//                 PDeviceId dev_id = this->locToId[u];
//               #ifdef SERIAL
//                 DPDState* state = cells.at(dev_id);
//               #else
//                 DPDState* state = &this->cells.devices[dev_id]->state;
//               #endif
//                 uint32_t bslot = state->bslot;
//                 while (bslot) {
//                     uint8_t i = get_next_slot(bslot);
//                     bead_t b = state->bead_slot[i];
//                     if (first) {
//                         first = false;
//                     } else {
//                         fprintf(f, ",\n");
//                     }
//                     fprintf(f, "\t %f", find_nearest_bead_distance(&b, u));
//                     bslot = clear_slot(bslot, i);
//                 }
//             }
//         }
//     }
//     fprintf(f, "\n]}");
//     fclose(f);
//     std::cerr << "Complete\n";
// }

#endif /* __XMLVOLUME_IMPL */
