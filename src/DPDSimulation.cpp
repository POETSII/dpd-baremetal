
#include "DPDSimulation.hpp"

#ifndef _DPD_SIMULATION_IMPL
#define _DPD_SIMULATION_IMPL

std::vector<Polymer_structure> DPDSimulation::combinePolymers(std::vector<Polymer_structure> a, std::vector<Polymer_structure> b) {
    for (std::vector<Polymer_structure>::iterator i = b.begin(); i != b.end(); ++i) {
        a.push_back(*i);
    }
    return a;
}

void printPolymerStructure(Polymer_structure p) {
    std::map<int, bool> openLoops;
    for (std::vector<Polymer_structure>::iterator i = p.elements.begin(); i != p.elements.end(); ++i) {
        if (i->type == BEAD) {
            std::cout << i->bead_type << " ";
        } else if (i->type == CHAIN) {
            printPolymerStructure(*i);
        } else if (i->type == BRANCH) {
            std::cout << "( BRANCH ";
            printPolymerStructure(*i);
            std::cout << ")";
        } else if (i->type == LOOP) {
            if (openLoops[i->loopNum]) {
                std::cout << ")";
                openLoops[i->loopNum] = false;
            } else {
                std::cout << i->bead_type << "( LOOP " << i->loopNum << " ";
                printPolymerStructure(*i);
                openLoops[i->loopNum] = true;
            }
        } else {
            std::cout << "PANIC\n";
        }
    }
}

// Polymer_structure, int for where to continue in string, ending loop number
std::tuple<Polymer_structure, int> DPDSimulation::parsePolymerStructure(std::string s, int start) {
    std::vector<Polymer_structure> polymers;
    bool loop = false;
    int repeatNum = 0, loopNum = 0;
    std::string numStr;
    bead_type_id loopBead;
    for (int c = start; c < s.size(); c++) {
        if (s[c] == '(') { // Open of new sub-structure
            std::tuple<Polymer_structure, int> r = parsePolymerStructure(s, c+1);
            polymers.push_back(std::get<0>(r));
            c = std::get<1>(r);
        }
        else if (s[c] == ' ') { // Whitespace could be ignored or might be important
            if (loop) {
                if (numStr == "") {
                    continue;
                }
                loopNum = std::stoi(numStr);
                numStr = "";
            } else if (numStr != "") {
                repeatNum = std::stoi(numStr);
                numStr = "";
            }
        }
        else if (s[c] == ')') { // End of sub-structure return the parse polymer
            Polymer_structure p;
            if (loop) {
                Polymer_structure p;
                p.type = LOOP;
                p.loopNum = loopNum;
                p.bead_type = loopBead;
                return std::make_tuple(p, c);
            } else {
                p.type = CHAIN;
                p.elements = polymers;
                return std::make_tuple(p, c);
            }
        }
        else if (s[c] == '*') { // Indicates a branch
            std::tuple<Polymer_structure, int> r = parsePolymerStructure(s, c+1);
            Polymer_structure returnStructure = std::get<0>(r);
            returnStructure.type = BRANCH;
            c = std::get<1>(r);
            polymers.push_back(returnStructure);
        }
        else if (s[c] == '/') { // Indicates the start or end of a loop
            loop = true;
        }
        else if (s[c] >= '0' && s[c] <= '9') { // Number for loop or repeats?
            numStr += s[c];
        }
        else if ((s[c] >= 'a' && s[c] <= 'z') || (s[c] >= 'A' && s[c] <= 'Z')) {
            if (loop) {
                loopBead = s[c];
            } else if (repeatNum == 0) {
                Polymer_structure n;
                n.type = BEAD;
                n.bead_type = s[c];
                polymers.push_back(n);
            } else {
                for (int i = 0; i < repeatNum; i++) {
                    Polymer_structure n;
                    n.type = BEAD;
                    n.bead_type = s[c];
                    polymers.push_back(n);
                }
                repeatNum = 0;
            }
        }
    }
    Polymer_structure p;
    p.type = CHAIN;
    p.elements = polymers;
    return std::make_tuple(p, s.size());
}

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

void DPDSimulation::addPolymer(Polymer p) {
    _polymers.push_back(p);
}

std::vector<Polymer> DPDSimulation::getPolymers() {
    return _polymers;
}

void DPDSimulation::setVolume(Volume volume) {
    _volume = volume;
}

Volume DPDSimulation::getVolume() {
    return _volume;
}

void DPDSimulation::setCell(Cell cell) {
    _cell = cell;
}

Cell DPDSimulation::getCell() {
    return _cell;
}

void DPDSimulation::setDensity(float density) {
    _density = density;
}

float DPDSimulation::getDensity() {
    return _density;
}

void DPDSimulation::setTemp(float temp) {
    _temp = temp;
}

float DPDSimulation::getTemp() {
    return _temp;
}

void DPDSimulation::setRNGSeed(int32_t rngSeed) {
    _RNGSeed = rngSeed;
}

int32_t DPDSimulation::getRNGSeed() {
    return _RNGSeed;
}

void DPDSimulation::setLambda(float lambda) {
    _lambda = lambda;
}

float DPDSimulation::getLambda() {
    return _lambda;
}

void DPDSimulation::setStep(float step) {
    _step = step;
}

float DPDSimulation::getStep() {
    return _step;
}

void DPDSimulation::setTime(uint32_t time) {
    _time = time;
}

uint32_t DPDSimulation::getTime() {
    return _time;
}

void DPDSimulation::setSamplePeriod(uint32_t samplePeriod) {
    _samplePeriod = samplePeriod;
}

uint32_t DPDSimulation::getSamplePeriod() {
    return _samplePeriod;
}

void DPDSimulation::setAnalysisPeriod(uint32_t analysisPeriod) {
    _analysisPeriod = analysisPeriod;
}

uint32_t DPDSimulation::getAnalysisPeriod() {
    return _analysisPeriod;
}

void DPDSimulation::setDensityPeriod(uint32_t densityPeriod) {
    _densityPeriod = densityPeriod;
}

uint32_t DPDSimulation::getDensityPeriod() {
    return _densityPeriod;
}

void DPDSimulation::setDisplayPeriod(uint32_t displayPeriod) {
    _displayPeriod = displayPeriod;
}

uint32_t DPDSimulation::getDisplayPeriod() {
    return _displayPeriod;
}

void DPDSimulation::setRestartPeriod(uint32_t restartPeriod) {
    _restartPeriod = restartPeriod;
}

uint32_t DPDSimulation::getRestartPeriod() {
    return _restartPeriod;
}

void DPDSimulation::setGrid(Grid grid) {
    _grid = grid;
}

Grid DPDSimulation::getGrid() {
    return _grid;
}

void DPDSimulation::setAnalysis(Analysis analysis) {
    _analysis = analysis;
}

Analysis DPDSimulation::getAnalysis() {
    return _analysis;
}

void DPDSimulation::addCommand(Command command) {
    _commands.push_back(command);
}

std::vector<Command> DPDSimulation::getCommands() {
    return _commands;
}

#endif /* _DPD_SIMULATION_IMPL */
