
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
