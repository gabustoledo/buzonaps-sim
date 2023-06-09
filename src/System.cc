#include "../includes/System.h"

System::System(EventList * _event_list) {}

void System::initializeCesfams() {
    Cesfam * cesfam = new Cesfam();
    // ?
    this->cesfams.insert({cesfam->getId(), cesfam});
}

void System::initializePatients() {
    Patient * patient;
    int i;
    for (i = 0; i < 20; i++) {
        patient = new Patient();
        // ?
        this->patients.insert({patient->getId(), patient});
        this->patients_vector.push_back(patient);
    }
}   

void System::initializeManagers() {
    Manager * manager = new Manager(this->cesfams.at(0), this->patients_vector);
    // ?
    this->managers.insert({manager->getId(), manager});
}

void System::processEvent(Event * e) {}

System::~System() {}
