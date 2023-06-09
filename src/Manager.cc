#include "../includes/Manager.h"

int Manager::_curr_id(0);

Manager::Manager(Cesfam * _cesfam, vector<Patient *> _patients) {
    this->cesfam = _cesfam;
    int i;
    for (i = 0; i < _patients.size(); i++) {
        this->patients.insert({_patients[i], false});
    }
}

int Manager::getId() {
    return this->id;
}