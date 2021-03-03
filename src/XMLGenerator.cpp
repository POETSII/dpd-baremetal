// Implementation file for the host simulation volume class

#include "XMLGenerator.hpp"

#ifndef __XMLGENERATOR_IMPL
#define __XMLGENERATOR_IMPL

XMLGenerator::XMLGenerator(const float volume_length, const unsigned cells_per_dimension, uint32_t start_timestep, uint32_t max_timestep, std::string out_path) : Simulator<XMLVolume>(volume_length, cells_per_dimension, start_timestep, max_timestep) {
    this->volume = new XMLVolume(volume_length, cells_per_dimension);
    this->out_path = out_path;
    this->graph_type = "../xml-graph-types/dpd_exfil_type.xml";
}

// Generate the XML
void XMLGenerator::run() {
    xml = generate_graph_instance();
}

// Generate the XML, put it in result for testing
void XMLGenerator::test(void *result) {

}

// Write the generated XML to a file
void XMLGenerator::write() {
    // Open the file
    FILE* f = fopen(out_path.c_str(), "w+");

    // Read the graph type and copy this into the file first
    std::ifstream t(graph_type);
    std::string line;
    // Get the first line of the file
    std::getline(t, line);

    while (line != "</Graphs>") {
        // Print this to the XML file
        fprintf(f, "%s\n", line.c_str());

        std::getline(t, line);
    }
    // "</Graphs>" should be last line of the file

    // Print the graph instance to the file now
    fprintf(f, "%s", xml.c_str());

    // Close the file with "</Graphs>"
    fprintf(f, "%s\n", line.c_str());

    // Close the file
    fclose(f);
}

std::string XMLGenerator::generate_graph_instance() {
    // Open tag
    std::string graphInstance = "\t<GraphInstance id=\"";

    // ID of this graph instance
    std::stringstream stream;
    stream << std::fixed << std::setprecision(0) << this->volume->get_volume_length();
    std::string vol_len = stream.str(); // The volume length in a short format
    graphInstance += "dpd_oil_water_" + vol_len + "_" + vol_len + "_" + vol_len;

    // Identify the graph type
    graphInstance += "\" graphTypeId=\"dpd_exfil_graph_type\"";

    // Set the properties
    graphInstance += " P=\"";

    // Get the cells to work with
    XMLCells *cells = (XMLCells *)this->volume->get_cells();
    GraphProperties *graphProperties = cells->get_graph_properties();

    // This is currently hard coded in the order found in the graph type
    graphInstance += std::to_string(graphProperties->r_c) + ", ";
    graphInstance += std::to_string(graphProperties->sq_r_c) + ", ";
    graphInstance += "{ ";
    for (unsigned i = 0; i < 3; i++) {
        graphInstance += "{";
        for (unsigned j = 0; j < 3; j++) {
            graphInstance += std::to_string(graphProperties->A[i][j]) + ", ";
        }
        graphInstance = graphInstance.substr(0, graphInstance.length() - 2);
        graphInstance += "}, ";
    }
    graphInstance = graphInstance.substr(0, graphInstance.length() - 2);
    graphInstance += " }, ";
    graphInstance += std::to_string(graphProperties->drag_coef) + ", ";
    graphInstance += std::to_string(graphProperties->sigma_ij) + ", ";
    graphInstance += std::to_string(graphProperties->dt_normal) + ", ";
    graphInstance += std::to_string(graphProperties->dt_early) + ", ";
    graphInstance += std::to_string(graphProperties->inv_sqrt_dt_normal) + ", ";
    graphInstance += std::to_string(graphProperties->inv_sqrt_dt_early) + ", ";
    graphInstance += std::to_string(graphProperties->lambda) + ", ";
    graphInstance += std::to_string(graphProperties->cell_length) + ", ";
    graphInstance += std::to_string(graphProperties->emitperiod) + ", ";
    graphInstance += std::to_string(start_timestep) + ", ";
    graphInstance += std::to_string(max_timestep) + ", ";
    graphInstance += std::to_string(graphProperties->cells_per_dimension) + ", ";
    graphInstance += std::to_string(volume->get_number_of_beads()) + "\"";

    // End tag and move to next line
    graphInstance += ">\n";
    // DEVICE INSTANCES
    graphInstance += generate_device_instances();

    // EDGE INSTANCES
    graphInstance += generate_edge_instances();

    graphInstance += "\t</GraphInstance>\n";

    return graphInstance;
}

std::string XMLGenerator::generate_device_instances() {
    std::string devIs = "\t\t<DeviceInstances>\n";
    for (uint16_t x = 0; x < this->volume->get_cells_per_dimension(); x++) {
        for (uint16_t y = 0; y < this->volume->get_cells_per_dimension(); y++) {
            for (uint16_t z = 0; z < this->volume->get_cells_per_dimension(); z++) {
                cell_t c = {x, y, z};
                devIs += generate_device_instance(c);
            }
        }
    }
    devIs += "\t\t</DeviceInstances>\n";
    return devIs;
}

std::string XMLGenerator::generate_device_instance(cell_t loc) {
    // Get the properties and state for this cell
    XMLCells *cells = (XMLCells *)this->volume->get_cells();
    DPDProperties *deviceProperties = cells->get_cell_properties(loc);
    DPDState *deviceState = cells->get_cell_state(loc);

    std::string devI = "\t\t\t<DevI "; // Open the tag
    // Give it an ID
    devI += "id=\"cell_" + std::to_string(loc.x) + "_" + std::to_string(loc.y) + "_" + std::to_string(loc.z) + "\" ";
    // Give it a type
    devI += "type=\"dpd_cell\" ";

    // Write it's properties
    devI += "P=\"";
    // It's only properties are cell location in the volume
    devI += "{" + std::to_string(deviceProperties->loc[0]) + ", " + std::to_string(deviceProperties->loc[1]) + ", " + std::to_string(deviceProperties->loc[2]) + "}";
    // Close the properties
    devI += "\" ";

    // Write it's state
    devI += "S=\"";
    // Cell state is hard-coded to match that in the non-generated graph type
    devI += std::to_string(deviceState->bslot) + ", ";
    devI += std::to_string(deviceState->sentslot) + ", ";
    devI += std::to_string(deviceState->newBeadMap) + ", ";
    devI += std::to_string(deviceState->migrateslot) + ", ";
    devI += "{";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += std::to_string(deviceState->bead_slot_id[i]) + ", ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += "}, {";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += std::to_string(deviceState->bead_slot_type[i]) + ", ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += "}, { ";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->bead_slot_pos[i][0]) + ", ";
        devI += std::to_string(deviceState->bead_slot_pos[i][1]) + ", ";
        devI += std::to_string(deviceState->bead_slot_pos[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, { ";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->bead_slot_vel[i][0]) + ", ";
        devI += std::to_string(deviceState->bead_slot_vel[i][1]) + ", ";
        devI += std::to_string(deviceState->bead_slot_vel[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, {";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->bead_slot_acc[i][0]) + ", ";
        devI += std::to_string(deviceState->bead_slot_acc[i][1]) + ", ";
        devI += std::to_string(deviceState->bead_slot_acc[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, {";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->force_slot[i][0]) + ", ";
        devI += std::to_string(deviceState->force_slot[i][1]) + ", ";
        devI += std::to_string(deviceState->force_slot[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, {";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->old_vel[i][0]) + ", ";
        devI += std::to_string(deviceState->old_vel[i][1]) + ", ";
        devI += std::to_string(deviceState->old_vel[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, {";
    for (unsigned i = 0; i < MAX_BEADS; i++) {
        devI += "{";
        devI += std::to_string(deviceState->migrate_loc[i][0]) + ", ";
        devI += std::to_string(deviceState->migrate_loc[i][1]) + ", ";
        devI += std::to_string(deviceState->migrate_loc[i][2]) + "}, ";
    }
    devI = devI.substr(0, devI.length() - 2);
    devI += " }, ";
    devI += std::to_string(deviceState->mode) + ", ";
    devI += std::to_string(deviceState->emitcnt) + ", ";
    devI += std::to_string(deviceState->timestep) + ", ";
    devI += std::to_string(deviceState->grand) + ", ";
    devI += std::to_string(deviceState->rngstate) + ", ";
    devI += std::to_string(deviceState->dt) + ", ";
    devI += std::to_string(deviceState->inv_sqrt_dt) + ", ";
    devI += std::to_string(deviceState->updates_received) + ", ";
    devI += std::to_string(deviceState->update_completes_received) + ", ";
    devI += std::to_string(deviceState->updates_sent) + ", ";
    devI += std::to_string(deviceState->total_update_beads) + ", ";
    devI += std::to_string(deviceState->migrations_received) + ", ";
    devI += std::to_string(deviceState->migration_completes_received) + ", ";
    devI += std::to_string(deviceState->migrates_sent) + ", ";
    devI += std::to_string(deviceState->total_migration_beads) + ", ";
    devI += std::to_string(deviceState->emit_complete_sent) + ", ";
    devI += std::to_string(deviceState->emit_completes_received);
    // Close the state
    devI += "\"";

    // Close the tag
    devI += " />\n";

    return devI;
}

std::string XMLGenerator::generate_edge_instances() {
    std::string edgeIs = "\t\t<EdgeInstances>\n";
    for (uint16_t x = 0; x < this->volume->get_cells_per_dimension(); x++) {
        for (uint16_t y = 0; y < this->volume->get_cells_per_dimension(); y++) {
            for (uint16_t z = 0; z < this->volume->get_cells_per_dimension(); z++) {
                cell_t c = {x, y, z};
                edgeIs += generate_input_edge_instances_for_cell(c);
            }
        }
    }
    // Close the section
    edgeIs += "\t\t</EdgeInstances>\n";
    return edgeIs;
}

std::string XMLGenerator::generate_input_edge_instances_for_cell(cell_t loc) {

    // Get cells neighbours and map from ID to loc
    XMLCells *cells = (XMLCells *)this->volume->get_cells();
    std::map<PDeviceId, cell_t> *idToLoc = cells->get_idToLoc();
    std::vector<PDeviceId> *neighbours = cells->get_cell_neighbours(loc);
    // Accumulate the edges here
    std::string this_cell = "cell_" + std::to_string(loc.x) + "_" + std::to_string(loc.y) + "_" + std::to_string(loc.z);
    std::string edgeI;

    for (std::vector<PDeviceId>::iterator n = neighbours->begin(); n != neighbours->end(); ++n) {
        cell_t n_loc = (*idToLoc)[*n];
        std::string n_cell = "cell_" + std::to_string(n_loc.x) + "_" + std::to_string(n_loc.y) + "_" + std::to_string(n_loc.z);
        // Hard coded edges
        edgeI += "\t\t\t<EdgeI path=\"" + this_cell + ":update_recv-" + n_cell + ":update_send\"/>\n";
        edgeI += "\t\t\t<EdgeI path=\"" + this_cell + ":update_complete_recv-" + n_cell + ":update_complete_send\"/>\n";
        edgeI += "\t\t\t<EdgeI path=\"" + this_cell + ":migrate_recv-" + n_cell + ":migrate_send\"/>\n";
        edgeI += "\t\t\t<EdgeI path=\"" + this_cell + ":migrate_complete_recv-" + n_cell + ":migrate_complete_send\"/>\n";
        edgeI += "\t\t\t<EdgeI path=\"" + this_cell + ":emit_complete_recv-" + n_cell + ":emit_complete_send\"/>\n";
    }

    return edgeI;
}

#endif /* __XMLGENERATOR_IMPL */
