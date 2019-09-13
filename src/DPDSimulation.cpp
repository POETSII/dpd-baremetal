
void DPDSimulation::setTitle(std::string title) {
    _title = title;
}

std::string DPDSimulation::getTitle() {
    return _title;
}

bool DPDSimulation::setDate(Date date) {
    bool valid = date.validate();
    if (valid) {
        _date = date;
    }
    return valid;
}

Date DPDSimulation::getDate() {
    return _date;
}

void DPDSimulation::setComment(std::string comment) {
    _comment = comment;
}

std::string DPDSimulation::getComment() {
    return _comment;
}

void DPDSimulation::setInitialState(initial_state state) {
    if (state != RANDOM) {
        printf("ERROR: Only random initial state setting is implemented. Continuing with RANDOM\n");
        _state = RANDOM;
    } else {
        _state = state;
    }
}

initial_state DPDSimulation::getInitialState() {
    return _state;
}

void DPDSimulation::addBeadType(bead_type_id id, Bead_type beadType) {
    _bead_types[id] = beadType;
}

std::map<bead_type_id, Bead_type> DPDSimulation::getBeadTypes() {
    return _bead_types;
}

void DPDSimulation::addConservativeParameters(std::vector<float> conservativeParameters) {
    // Mirror the matrix as it is inserted
    if (conservativeParameters.size() > 1) {
        std::vector<std::vector<float>>::iterator i = _conservative_parameters.begin();
        for (std::vector<float>::iterator j = conservativeParameters.begin(); j != conservativeParameters.end(); ++j) {
            i->push_back(*j);
            i = std::next(i);
            if (i == _conservative_parameters.end()) {
                break;
            }
        }
    }
    // Add the new list of conservative parameters
    _conservative_parameters.push_back(conservativeParameters);
}

std::vector<std::vector<float>> DPDSimulation::getConservativeParameters() {
    return _conservative_parameters;
}

void DPDSimulation::addDissipativeParameters(std::vector<float> dissipativeParameters) {
    // Mirror the matrix as it is inserted
    if (dissipativeParameters.size() > 1) {
        std::vector<std::vector<float>>::iterator i = _dissipative_parameters.begin();
        for (std::vector<float>::iterator j = dissipativeParameters.begin(); j != dissipativeParameters.end(); ++j) {
            i->push_back(*j);
            i = std::next(i);
            if (i == _dissipative_parameters.end()) {
                break;
            }
        }
    }
    // Add the new list of dissipative parameters
    _dissipative_parameters.push_back(dissipativeParameters);
}

std::vector<std::vector<float>> DPDSimulation::getDissipativeParameters() {
    return _dissipative_parameters;
}

void DPDSimulation::addBondType(Bond_type bondType) {
    _bond_types.push_back(bondType);
}

std::vector<Bond_type> DPDSimulation::getBondTypes() {
    return _bond_types;
}

void DPDSimulation::addStiffBondType(Stiff_bond_type stiffBondType) {
    _stiff_bond_types.push_back(stiffBondType);
}

std::vector<Stiff_bond_type> DPDSimulation::getStiffBondTypes() {
    return _stiff_bond_types;
}
