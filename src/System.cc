#include "../includes/System.h"

System::System(EventList * _event_list) {
    this->event_list = _event_list;
}

void System::log(json obj)
{
    this->monitor->writeLog(obj);
}

void System::initializeCesfams() {
    Cesfam * cesfam = new Cesfam();
    // ?
    this->cesfams.insert({cesfam->getId(), cesfam});
    printf("Saliendo de initializeCesfams");
}

void System::initializePatients(int amount) {
    int i;
    for (i = 0; i < amount; i++) {
        Patient * patient = new Patient(this->event_list, this);
        // ?
        this->patients.insert({patient->getId(), patient});
        this->patients_vector.push_back(patient);
    }
    printf("Cantidad de pacientes: %ld - i: %d\n", this->patients.size(), i);
    printf("Saliendo de initializePatients\n");
}   

void System::initializeManagers() {
    Manager * manager = new Manager(this->cesfams.at(1), this->patients_vector, this->event_list, this);
    size_t i;
    for(i = 0; i < this->patients_vector.size(); i++) {
        this->patients_vector[i]->setManager(manager);
    }
    // ?
    this->managers.insert({manager->getId(), manager});
    printf("Saliendo de initializeManagers\n");
}

void System::initializeSystem(int patients_amount) {
    printf("Entrando a initializeSystem\n");
    this->initializeCesfams();
    this->initializePatients(patients_amount);
    this->initializeManagers();

    // Se configura el monitor
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream time;
    time << put_time(&tm, "%d%m%Y_%H-%M-%S");
    string output_str = "./out/out_" + time.str() + ".dat";
    Monitor *monitor = new Monitor(output_str);
    this->monitor = monitor;

    // Se disparan los primeros eventos en base a los pacientes existentes
    Event * e;
    int i = 0;
    printf("Cantidad de pacientes: %ld \n", this->patients.size());
    for (auto p = this->patients.begin(); p != this->patients.end(); ++p) {
        e = new Event(CallerType::AGENT_MANAGER, p->first, (ASK_CONSENT_TIME * i), ASK_CONSENT_TIME, 
                    ManagerEvents::ASK_CONSENT, p->second, p->second->getManager(), nullptr);
        printf("Evento: %d, puntero: %p, puntero manager: %p\n", i, e, p->second->getManager());
        this->event_list->insertEvent(e);
        i++;
    }
    printf("Saliendo de initilizeSystem\n");
}

void System::processEvent(Event * e) {}

System::~System() {}
