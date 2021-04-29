// Implementation file for the host SimulationVolume class

#include "SimulationVolume.hpp"

#ifndef __SIM_VOLUME_IMPL
#define __SIM_VOLUME_IMPL

// constructor
template<class V>
SimulationVolume<V>::SimulationVolume(const float volume_length, const unsigned cells_per_dimension) : Volume<V>(volume_length, cells_per_dimension) {

#ifdef GALS
    std::cout << "Building a GALS volume.\n";
#elif defined(SERIAL)
    std::cout << "Building a serial volume.\n";
    std::cout << "The DPD algorithm will be run on this x86 machine in serial.\n";
#elif defined(XML)
    std::cout << "Building an XML file to be run with The Ochestrator.\n";
#else
    std::cout << "Building a synchronous volume.\n";
#endif

#if defined(ONE_BY_ONE)
    std::cout << "Local bead interactions will be performed one at a time (OneByOne).\n";
#elif defined(SEND_TO_SELF)
    std::cout << "Cells will send beads to themselves to calculate bead interactions (SendToSelf).\n";
#endif

#ifdef MESSAGE_MANAGEMENT
    std::cout << "Messages are sent at intervals determined by the number of messages sent by neighbours (MessageManagement).\n";
#endif

#ifdef DOUBLE_SQRT
    std::cout << "Sqrt will run double number of calculations. NOTE: This will fail testing\n";
#endif

#ifdef REDUCED_LOCAL_CALCS
    std::cout << "Local bead interactions will be reduced.\n";
    std::cout << "Calculating the force between two local beads will be added to one bead and subtracted from the other.\n";
#endif

}

// deconstructor
// template<class V>
// SimulationVolume<V>::~SimulationVolume() {
// //    Volume<S, PGraph<DPDDevice, DPDState, None, DPDMessage> *>::~Volume();
// }

// Checks to see if a bead can be added to the volume
template<class V>
bool SimulationVolume<V>::space_for_bead(const bead_t *in) {
    bead_t b = *in;
    float cell_length = this->cells->get_cell_length();
    cell_pos_t x = floor(b.pos.x()/cell_length);
    cell_pos_t y = floor(b.pos.y()/cell_length);
    cell_pos_t z = floor(b.pos.z()/cell_length);

    cell_t t = {x,y,z};
    if (x >= this->volume_length || x < 0 || y >= this->volume_length || y < 0 || z >= this->volume_length || z < 0) {
        return false;
    }

    // Check to make sure there is still enough room in the device
    if(get_num_beads(this->cells->get_cell_bslot(t)) >= MAX_BEADS) {
        return false;
    }

    // Adjust the bead position
    b.pos.x(b.pos.x() - x);
    b.pos.y(b.pos.y() - y);
    b.pos.z(b.pos.z() - z);

    if (find_nearest_bead_distance(&b, t) < 0.45) {
        return false;
    }

    return true;
}

// Checks to see if a pair of beads can be added to the volume
template<class V>
bool SimulationVolume<V>::space_for_bead_pair(const bead_t *pa, const bead_t *pb) {
    cell_pos_t xa = floor(pa->pos.x() / this->cell_length);
    cell_pos_t ya = floor(pa->pos.y() / this->cell_length);
    cell_pos_t za = floor(pa->pos.z() / this->cell_length);
    cell_t ta = {xa,ya,za};

    cell_pos_t xb = floor(pb->pos.x() / this->cell_length);
    cell_pos_t yb = floor(pb->pos.y() / this->cell_length);
    cell_pos_t zb = floor(pb->pos.z() / this->cell_length);
    cell_t tb = {xb,yb,zb};

    if (this->locToId.find(ta) == this->locToId.end()){
        return false;
    }

    if (this->locToId.find(tb) == this->locToId.end()){
        return false;
    }

    // lookup the device
   PDeviceId b_sua = this->locToId[ta];
   PDeviceId b_sub = this->locToId[tb];

    if (b_sua == b_sub) {
      #ifdef SERIAL
        return get_num_beads(this->cells.at(b_sua)->bslot) + 1 < MAX_BEADS;
      #else
        return get_num_beads(this->cells.devices[b_sua]->state.bslot) + 1 < MAX_BEADS;
      #endif
    } else {
      #ifdef SERIAL
        return (get_num_beads(this->cells.at(b_sua)->bslot) < MAX_BEADS) && (get_num_beads(this->cells.getCell(b_sub)->bslot) < MAX_BEADS);
      #else
        return (get_num_beads(this->cells.devices[b_sua]->state.bslot) < MAX_BEADS) && (get_num_beads(this->cells.devices[b_sub]->state.bslot) < MAX_BEADS);
      #endif
   }
}

template<class V>
uint16_t SimulationVolume<V>::get_neighbour_cell_dimension(cell_pos_t c, int16_t n) {
    if (n == -1) {
        if (c == 0) {
            return this->cells->get_cells_per_dimension() - 1;
        } else {
            return c - 1;
        }
    } else if (n == 1) {
        if (c == this->cells->get_cells_per_dimension() - 1) {
            return 0;
        } else {
            return c + 1;
        }
    } else {
        return c;
    }
}

template<class V>
cell_t SimulationVolume<V>::get_neighbour_cell_loc(cell_t u_i, int16_t d_x, int16_t d_y, int16_t d_z) {
    cell_t u_j = {
        get_neighbour_cell_dimension(u_i.x, d_x),
        get_neighbour_cell_dimension(u_i.y, d_y),
        get_neighbour_cell_dimension(u_i.z, d_z)
    };
    return u_j;
}

template<class V>
float SimulationVolume<V>::find_nearest_bead_distance(const bead_t *i, cell_t u_i) {
    float min_dist = 100.0;
    for (int16_t d_x = -1; d_x <= 1; d_x++) {
        for (int16_t d_y = -1; d_y <= 1; d_y++) {
            for (int16_t d_z = -1; d_z <= 1; d_z++) {
                cell_t n_loc = get_neighbour_cell_loc(u_i, d_x, d_y, d_z);

                // Get neighbour bead slot
                uint8_t nslot = this->cells->get_cell_bslot(n_loc);
                while (nslot) {
                    uint8_t cj = get_next_slot(nslot);
                    nslot = clear_slot(nslot, cj);
                    const bead_t *j = this->cells->get_bead_from_cell_slot(n_loc, cj);
                    if (j->id == i->id) {
                        continue;
                    }
                    Vector3D<float> j_adj; // Adjust the neighbour bead, j, relative to the given bead, i
                    j_adj.x(j->pos.x() + d_x);
                    j_adj.y(j->pos.y() + d_y);
                    j_adj.z(j->pos.z() + d_z);
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

template<class V>
void SimulationVolume<V>::store_initial_bead_distances() {
    std::cerr << "Outputting minimum distances between beads for initial placement to ../init_dist.json\n";
    FILE* f = fopen("../init_dist.json", "w+");
    fprintf(f, "{ \"min_dists\":[\n");
    bool first = true;
    for (cell_pos_t u_x = 0; u_x < this->cells_per_dimension; u_x++) {
        for (cell_pos_t u_y = 0; u_y < this->cells_per_dimension; u_y++) {
            for (cell_pos_t u_z = 0; u_z < this->cells_per_dimension; u_z++) {
                cell_t u = { u_x, u_y, u_z };

                uint8_t bslot = this->cells->get_cell_bslot(u);
                while (bslot) {
                    uint8_t i = get_next_slot(bslot);
                    bead_t b = this->cells->get_bead_from_cell_slot(u, i);
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

#endif /* __SIM_VOLUME_IMPL */
