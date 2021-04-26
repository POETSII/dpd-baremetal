// Holds cell information and provides access to cell states.
// Provides an interface for PGraph

#include "XMLCells.hpp"

#ifndef __XML_CELLS_IMPL
#define __XML_CELLS_IMPL

#include <iostream>

// Constructor
XMLCells::XMLCells(unsigned cells_per_dimension, ptype cell_length) : SimulationCells<std::vector<DPDState>>(cells_per_dimension, cell_length) {

    // Set the graph properties
    graph_properties.r_c = r_c;
    graph_properties.sq_r_c = 1.0;
    for (unsigned i = 0; i < BEAD_TYPES; i++) {
        for (unsigned j = 0; j < BEAD_TYPES; j++) {
            graph_properties.A[i][j] = A[i][j];
        }
    }
    graph_properties.drag_coef = drag_coef; // Drag coefficient
    graph_properties.sigma_ij = sigma_ij; // Random coefficient. Related to drag_coef and temperature - simga = sqrt(2 * drag_coef * temp)
    graph_properties.dt_normal = normal_dt; // Size of timestep after 1000 timesteps
    graph_properties.dt_early  = early_dt; // Size of timestep for first 1000 timesteps
    graph_properties.inv_sqrt_dt_normal = normal_inv_sqrt_dt; // Inverse sqrt of dt after 1000 timesteps
    graph_properties.inv_sqrt_dt_early  = early_inv_sqrt_dt; // Inverse sqrt of dt for first 1000 timesteps
    graph_properties.lambda = lambda; // Lambda used in thermostat
    graph_properties.cell_length = cell_length;
    graph_properties.cells_per_dimension = cells_per_dimension;

    // These are initialised to 0 now, but set later before being written
    graph_properties.emitperiod = 1; // Frequency of the state being output to the host
    graph_properties.start_timestep = 0;
    graph_properties.max_timestep = 0; // Number of timesteps to simulate
    graph_properties.total_beads = 0; // This will be set later

    // Create a device for each cell
    for(uint16_t x = 0; x < cells_per_dimension; x++) {
        for(uint16_t y = 0; y < cells_per_dimension; y++) {
            for(uint16_t z = 0; z < cells_per_dimension; z++) {
                    PDeviceId id = cells.size();
                    // Create state
                    DPDState state;
                    cells.push_back(state);
                    // Create properties
                    DPDProperties props;
                    cell_properties.push_back(props);
                    // Update the mapping
                    cell_t loc = {x, y, z};
                    this->idToLoc[id] = loc;
                    this->locToId[loc] = id;
            }
        }
    }

    connect_cells(cells_per_dimension);

    initialise_cells();
}

// Destructor
XMLCells::~XMLCells() {

}

void XMLCells::write(void *dest) {

}

void XMLCells::set_start_timestep(uint32_t start_timestep) {
    graph_properties.start_timestep = start_timestep;
}

void XMLCells::set_end_timestep(uint32_t end_timestep) {
    graph_properties.max_timestep = end_timestep;
}

DPDState * XMLCells::get_cell_state(cell_t loc) {
    return &cells.at(locToId[loc]);
}

DPDProperties * XMLCells::get_cell_properties(cell_t loc) {
    return &cell_properties.at(locToId[loc]);
}

GraphProperties * XMLCells::get_graph_properties() {
    return &graph_properties;
}

std::vector<PDeviceId> * XMLCells::get_cell_neighbours(cell_t loc) {
    return &neighbours[locToId[loc]];
}

uint8_t XMLCells::get_cell_bslot(cell_t loc) {
    return this->cells.at(locToId[loc]).bslot;
}

uint8_t XMLCells::get_device_bslot(PDeviceId id) {
    return this->cells.at(id).bslot;
}

const bead_t * XMLCells::get_bead_from_cell_slot(cell_t loc, uint8_t slot) {
    DPDState *state = &this->cells.at(locToId[loc]);
    bead_t b;
    b.id = state->bead_slot_id[slot];
    b.type = state->bead_slot_type[slot];
    b.pos.x(state->bead_slot_pos[slot][0]);
    b.pos.y(state->bead_slot_pos[slot][1]);
    b.pos.z(state->bead_slot_pos[slot][2]);
    b.velo.x(state->bead_slot_vel[slot][0]);
    b.velo.y(state->bead_slot_vel[slot][1]);
    b.velo.z(state->bead_slot_vel[slot][2]);
    b.acc.x(state->bead_slot_acc[slot][0]);
    b.acc.y(state->bead_slot_acc[slot][1]);
    b.acc.z(state->bead_slot_acc[slot][2]);
}

const bead_t * XMLCells::get_bead_from_device_slot(PDeviceId id, uint8_t slot) {
    DPDState *state = &this->cells.at(id);
    bead_t b;
    b.id = state->bead_slot_id[slot];
    b.type = state->bead_slot_type[slot];
    b.pos.x(state->bead_slot_pos[slot][0]);
    b.pos.y(state->bead_slot_pos[slot][1]);
    b.pos.z(state->bead_slot_pos[slot][2]);
    b.velo.x(state->bead_slot_vel[slot][0]);
    b.velo.y(state->bead_slot_vel[slot][1]);
    b.velo.z(state->bead_slot_vel[slot][2]);
    b.acc.x(state->bead_slot_acc[slot][0]);
    b.acc.y(state->bead_slot_acc[slot][1]);
    b.acc.z(state->bead_slot_acc[slot][2]);
}

void XMLCells::place_bead_in_cell(bead_t *b, cell_t loc) {
    place_bead_in_device(b, locToId[loc]);
}

void XMLCells::place_bead_in_device(bead_t *b, PDeviceId id) {
    DPDState *state = &this->cells.at(id);
    uint8_t slot = get_next_free_slot(state->bslot);
    state->bead_slot_id[slot] = b->id;
    state->bead_slot_type[slot] = b->type;
    state->bead_slot_pos[slot][0] = b->pos.x();
    state->bead_slot_pos[slot][1] = b->pos.y();
    state->bead_slot_pos[slot][2] = b->pos.z();
    state->bead_slot_vel[slot][0] = b->velo.x();
    state->bead_slot_vel[slot][1] = b->velo.y();
    state->bead_slot_vel[slot][2] = b->velo.z();
    state->bead_slot_acc[slot][0] = b->acc.x();
    state->bead_slot_acc[slot][1] = b->acc.y();
    state->bead_slot_acc[slot][2] = b->acc.z();

    state->bslot = set_slot(state->bslot, slot); // Set the slot
    state->sentslot = state->bslot; // Set sentslot so the beads is shared at timestep 0
}

void XMLCells::addNeighbour(PDeviceId a, PDeviceId b) {
    neighbours[a].push_back(b);
}

void XMLCells::initialise_cells() {

    // Place all cell locations in its state
    for (std::map<PDeviceId, cell_t>::iterator i = this->idToLoc.begin(); i != this->idToLoc.end(); ++i) {
        // Device ID
        PDeviceId id = i->first;
        // Related cell_t identifying it's position in the volume
        cell_t loc = i->second;

        // Get its state
        DPDState *state = &this->cells.at(id);
        DPDProperties *props = &cell_properties.at(id);

        // Set its location in the volume
        props->loc[0] = loc.x;
        props->loc[1] = loc.y;
        props->loc[2] = loc.z;

        // Some state can be set now
        state->mode = UPDATE; // First mode is update
        state->rngstate = 1234; // start with a seed
        state->dt = early_dt;
        state->inv_sqrt_dt = early_inv_sqrt_dt;
        state->emitcnt = 1; // Don't emit until emitperiod has been reached

        // Zero all remaining state
        state->bslot = 0;
        state->sentslot = 0;
        state->newBeadMap = 0;
        state->migrateslot = 0;
        state->timestep = 0;
        state->grand = 0;
        state->updates_received = 0;
        state->update_completes_received = 0;
        state->updates_sent = 0;
        state->total_update_beads = 0;

        state->migrations_received = 0;
        state->migration_completes_received = 0;
        state->migrates_sent = 0;
        state->total_migration_beads = 0;

        state->emit_complete_sent = 0;
        state->emit_completes_received = 0;

        for (unsigned i = 0; i < MAX_BEADS; i++) {
            state->bead_slot_id[i] = 0;
            state->bead_slot_type[i] = 0;
            for (unsigned j = 0; j < 3; j++) {
                state->bead_slot_pos[i][j] = 0;
                state->bead_slot_vel[i][j] = 0;
                state->bead_slot_acc[i][j] = 0;
                state->force_slot[i][j] = 0;
                state->old_vel[i][j] = 0;
                state->migrate_loc[i][j] = 0;
            }
        }

    }
}


#endif /* __XML_CELLS_IMPLE */
