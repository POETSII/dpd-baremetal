// Implementation of the RDFCalculator class.
// This can be run alongside a simulation, or on its own, as threads
// This will take a volume at a certain state, and calculate the RDF for all
// types.

#include "RDFCalculator.hpp"

#ifndef __RDFCALCULATOR_IMPL
#define __RDFCALCULATOR_IMPL

#include <unistd.h>

RDFCalculator::RDFCalculator(double volume_length, unsigned cells_per_dimension, uint32_t timestep, uint8_t number_density, uint8_t number_bead_types, std::vector<std::vector<std::vector<double>>> *results, moodycamel::BlockingConcurrentQueue<RDFMessage> *message_queue) : Executor(volume_length, cells_per_dimension) {
    for (uint8_t x = 0; x < this->volume.get_volume_length(); x++) {
        for (uint8_t y = 0; y < this->volume.get_volume_length(); y++) {
            for (uint8_t z = 0; z < this->volume.get_volume_length(); z++) {
                cell_t loc = {x, y, z};
                DPDState *state = this->volume.get_state_of_cell(loc);
                state->loc.x = loc.x;
                state->loc.y = loc.y;
                state->loc.z = loc.z;
                state->done = false;
            }
        }
    }

    this->timestep = timestep;
    this->number_density = number_density;
    this->number_bead_types = number_bead_types;
    this->results = results;
    this->message_queue = message_queue;

    this->rmax = this->volume.get_volume_length() / 2;
    this->min_r = -(ceil(this->volume.get_volume_length() / 2));
    this->max_r = ceil(this->volume.get_volume_length() / 2);
    this->dr = rmax / 100;
}

cell_t RDFCalculator::getNeighbourLoc(cell_t c, uint16_t n_x, uint16_t n_y, uint16_t n_z) {
    cell_t r;
    r.x = c.x + n_x;
    r.y = c.y + n_y;
    r.z = c.z + n_z;

    if (r.x < 0) {
        r.x = r.x + this->volume.get_volume_length();
    } else
    if (r.x >= 0) {
        if (r.x >= this->volume.get_volume_length()){
            r.x = r.x - this->volume.get_volume_length();
        }
    } else {
        r.x = c.x + r.x;
    }

    if (r.y < 0) {
        r.y = this->volume.get_volume_length() + r.y;
    } else
    if (r.y >= 0) {
        if (r.y >= this->volume.get_volume_length()) {
            r.y = r.y - this->volume.get_volume_length();
        }
    } else {
        r.y = c.y + r.y;
    }

    if (r.z < 0) {
        r.z = this->volume.get_volume_length() + r.z;
    } else
    if (r.z >= 0) {
        if (r.z >= this->volume.get_volume_length()) {
            r.z = r.z - this->volume.get_volume_length();
        }
    } else {
        r.z = c.z + r.z;
    }

    return r;
}

int16_t RDFCalculator::period_bound_adj(int16_t dim) {
    if (dim > max_r) {
        return -(volume.get_volume_length() - dim);
    } else if (dim < min_r) {
        return volume.get_volume_length() + dim;
    }
    return dim;
}

// Run the RDF calculations
void RDFCalculator::run() {
    uint64_t reference_beads[5] = {0, 0, 0, 0, 0};
    uint64_t type_nums[5][5][500];
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < 5; j++) {
            for (uint16_t k = 0; k < 500; k++) {
                type_nums[i][j][k] = 0;
            }
        }
    }
    uint32_t done_cells = 0;
    uint32_t total_cells = volume.get_number_of_cells();
    // Iterate through each cell
    for (std::vector<DPDState>::iterator state = volume.get_cells()->begin(); state != volume.get_cells()->end(); ++state) {
        // Current cell
        cell_t loc = state->loc;
        uint32_t done = 0;
        // Iterate through all neighbours of this cell
        for (uint n_x = 0; n_x < max_r + 1; n_x++) {
            for (uint n_y = 0; n_y < max_r + 1; n_y++) {
                for (uint n_z = 0; n_z < max_r + 1; n_z++) {
                    // Neighbour of current cell
                    cell_t n = getNeighbourLoc(loc, n_x, n_y, n_z);
                    DPDState *n_state = volume.get_state_of_cell(n);
                    // Check if the current cell has already been tested against the neighbouring cell
                    if (!n_state->done) {
                        // For each local bead
                        uint16_t i = state->bslot;
                        while(i) {
                            uint8_t ci = get_next_slot(i);
                            bead_t b_i = state->bead_slot[ci];
                            // For each bead in neighbour
                            uint16_t j = n_state->bslot;
                            while (j) {
                                uint8_t cj = get_next_slot(j);
                                bead_t b_j = n_state->bead_slot[cj];
                                // Neighbour can be the same as the cell so don't calculate distance between same bead
                                if (b_i.id != b_j.id) {
                                    // Adjust the position of the neighbour bead relative to the current cell bead
                                    int16_t x_rel = period_bound_adj(n_state->loc.x - state->loc.x);
                                    int16_t y_rel = period_bound_adj(n_state->loc.y - state->loc.y);
                                    int16_t z_rel = period_bound_adj(n_state->loc.z - state->loc.z);
                                    Vector3D<float> adj_j = Vector3D<float>(b_j.pos.x() + x_rel, b_j.pos.y() + y_rel, b_j.pos.z() + z_rel);
                                    // Get the Euclidean distance to between beads
                                    double dist = b_i.pos.dist(adj_j);
                                    if (dist < rmax) {
                                        // For each shell distance
                                        double r = 0;
                                        uint16_t index = floor(dist / dr);
                                        uint8_t min_type = std::min(b_i.type, b_j.type);
                                        uint8_t max_type = std::max(b_i.type, b_j.type);
                                        type_nums[min_type][max_type][index] += 2;
                                    }
                                }
                                j = clear_slot(j, cj);
                            }
                            i = clear_slot(i, ci);
                        }
                    } else {
                        done++;
                    }
                }
            }
        }
        state->done = true;
        // Add types of this cell to reference_beads
        uint16_t i = state->bslot;
        while (i) {
            uint8_t ci = get_next_slot(i);
            bead_t b_i = state->bead_slot[ci];
            reference_beads[b_i.type]++;
            i = clear_slot(i, ci);
        }

        uint32_t total_ref_beads = 0;
        for (uint8_t type = 0; type < this->number_bead_types; type++) {
            total_ref_beads += reference_beads[type];
        }
        double percent = (1000 * ((double)done_cells / volume.get_number_of_cells()));
        if (percent == floor(percent)) {
            RDFMessage msg = {timestep, running, percent / 10, sched_getcpu()};
            send_message(msg);
            // std::cout << "\tTimestep " << timestep << ": Cell " << done_cells << "/" << total_cells << ". ";
            // std::cout << "                                                 \n";
        }
        done_cells++;
    }
    // All beads have had all shells checked
    // Now lets calculate the values
    double r = 0;
    uint16_t index = 0;
    while (r <= rmax) {
        // r is inner radius, r_dr is outer radius
        const double r_dr = r + dr;
        // Volume of shell: Volume of outer sphere - volume of inner sphere
        const double volume = (4 / 3) * M_PI * (pow(r_dr, 3) - pow(r, 3));
        // Average over reference beads
        uint8_t i = 0;
        for (std::vector<std::vector<std::vector<double>>>::iterator outer = results->begin(); outer != results->end(); ++outer) {
            uint8_t j = i;
            for (std::vector<std::vector<double>>::iterator inner = outer->begin(); inner != outer->end(); ++inner) {
                uint32_t ref_beads = 0;
                if (i == j) {
                    ref_beads = reference_beads[i];
                } else {
                    ref_beads = reference_beads[i] + reference_beads[j];
                }
                // Get average
                double average = float(type_nums[i][j][index]) / ref_beads;
                // Divide average by volume - Accounts for volumes being larger the further out you look
                double g1 = average / volume;
                // Divide by number density
                double g2 = g1 / number_density;
                // Store this in the relevant array
                inner->push_back(g2);
                j++;
            }
            i++;
        }
        // Next shell
        r = r + dr;
        index++;
    }
    // Send message indicating we are complete
    RDFMessage msg;
    msg.progress = finished;
    msg.percent = 100.0;
    msg.timestep = timestep;
    msg.core = sched_getcpu();
    send_message(msg);
}

// Runs a test, returns the RDF values for comparison
void RDFCalculator::test(void *result) {

}

void RDFCalculator::send_message(RDFMessage msg) {
    message_queue->enqueue(msg);
}

uint32_t RDFCalculator::get_timestep() {
    return timestep;
}

#endif /* __RDFCalculator_IMPL */
