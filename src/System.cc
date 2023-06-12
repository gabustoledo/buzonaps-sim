#include "../includes/System.h"

System::System(EventList * _event_list) {
    this->event_list = _event_list;
}

void System::initializeCesfams() {
    Cesfam * cesfam = new Cesfam();
    // ?
    this->cesfams.insert({cesfam->getId(), cesfam});
    printf("Saliendo de initializeCesfams");
}

void System::initializePatients() {
    int i;
    for (i = 0; i < 3; i++) {
        Patient * patient = new Patient();
        // ?
        this->patients.insert({patient->getId(), patient});
        this->patients_vector.push_back(patient);
    }
    printf("Cantidad de pacientes: %ld - i: %d\n", this->patients.size(), i);
    printf("Saliendo de initializePatients\n");
}   

void System::initializeManagers() {
    Manager * manager = new Manager(this->cesfams.at(1), this->patients_vector, this->event_list);
    size_t i;
    for(i = 0; i < this->patients_vector.size(); i++) {
        this->patients_vector[i]->setManager(manager);
    }
    // ?
    this->managers.insert({manager->getId(), manager});
    printf("Saliendo de initializeManagers\n");
}

void System::initializeSystem() {
    printf("Entrando a initializeSystem\n");
    this->initializeCesfams();
    this->initializePatients();
    this->initializeManagers();

    // Se disparan los primeros eventos en base a los pacientes existentes
    Event * e;
    int i = 0;
    printf("Cantidad de pacientes: %ld \n", this->patients.size());
    for (auto p = this->patients.begin(); p != this->patients.end(); ++p) {
        e = new Event(CallerType::AGENT, p->first, (ASK_CONSENT_TIME * i), ASK_CONSENT_TIME, ManagerEvents::ASK_CONSENT, p->second, p->second->getManager(), nullptr);
        printf("Evento: %d, puntero: %p, puntero manager: %p\n", i, e, p->second->getManager());
        this->event_list->insertEvent(e);
        i++;
    }
    printf("Saliendo de initilizeSystem\n");
}

void System::processEvent(Event * e) {}

System::~System() {}
