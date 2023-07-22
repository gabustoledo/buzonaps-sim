#include "../includes/System.h"

System::System(EventList * _event_list) {
    this->event_list = _event_list;
}

void System::log(json obj)
{
    this->monitor->writeLog(obj);
}

void System::initializeCesfams(int amount) {
    int i;
    for (i = 0; i < amount; i++) {
        Cesfam * cesfam = new Cesfam();
        this->cesfams.insert({cesfam->getId(), cesfam});
        this->locations_cesfam.insert({i, cesfam});
    }
    printf("Saliendo de initializeCesfams");
}

void System::initializePatients(int amount, int cesfam_amount) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, cesfam_amount);
    int location;
    int i;
    for (i = 0; i < amount; i++) {
        location = dist(gen);
        Patient * patient = new Patient(this->event_list, this, location);
        // ?
        this->patients.insert({patient->getId(), patient});
        this->addLocationsPatients(location, patient);
    }
    printf("Cantidad de pacientes: %ld - i: %d\n", this->patients.size(), i);
    printf("Saliendo de initializePatients\n");
}   

void System::initializeManagers(int amount, int cesfam_amount) {
    
    int i;
    int ces;
    for (ces = 0; ces < cesfam_amount; ces++) {
        for (i = 0; i < amount; i++) {
            Manager * manager = new Manager(this->locations_cesfam.find(i)->second, this->event_list, this, ces);
            this->managers.insert({manager->getId(), manager});
            this->addLocationsManagers(ces, manager);
        }
    }

    int j;
    int k;
    int p;
    // Itero por las locaciones
    for (auto it = this->locations_managers.begin(); it != this->locations_managers.end(); it++) {
        int patients_vector_size = this->locations_patients.find(it->first)->second.size();
        int managers_vector_size = this->locations_managers.find(it->first)->second.size();
        int patients_per_manager = patients_vector_size/managers_vector_size;
        // printf("Patients vector size: %d \n", patients_vector_size);
        // printf("Managers vector size: %d \n", managers_vector_size);
        // printf("patients_per_manager: %d \n", patients_per_manager);
        k = 0;
        // Itero por los managers de la locación
        for (p = 0; p < managers_vector_size; p++) {
            int limit;
            // Si es el último ciclo, el máximo es la cantidad de pacientes
            if (p+1 == managers_vector_size) {
                limit = patients_vector_size;
            } else {
                limit = patients_per_manager*(p+1);
            }
            // printf("limit: %d \n", limit);
            // printf("k = %d \n", k);
            // Itero por los pacientes de esa locación
            for (j = k; j < limit; j++) {
                // Se agrega el paciente al manager
                this->locations_managers.find(it->first)->second[p]->addPatient(this->locations_patients.find(it->first)->second[j]);
                // Se setea el manager del paciente al manager actual
                this->locations_patients.find(it->first)->second[j]->setManager(this->locations_managers.find(it->first)->second[p]);
            }
            k = j;
        }
    }

    // map<Patient *, bool> patients;
    // for (auto o = this->managers.begin(); o != this->managers.end(); o++) {
    //     printf("MANAGER ID: %d - location: %d \n", o->second->getId(), o->second->getLocation());
    //     patients = o->second->getPatients();
    //     for (auto h = patients.begin(); h != patients.end(); h++) {
    //         printf("\t Patient ID: %d - location: %d - manager ptr: %p \n", h->first->getId(), h->first->getLocation(), h->first->getManager());
    //     }
    // }

    // for (auto h = this->patients.begin(); h != this->patients.end(); h++) {
    //     printf("\t Patient ID: %d - location: %d - manager ptr: %p \n", h->second->getId(), h->second->getLocation(), h->second->getManager());
    // }

    // exit(EXIT_SUCCESS);
    printf("Saliendo de initializeManagers\n");
}

void System::addLocaltionsCesfam(int location, Cesfam * cesfam) {
    this->locations_cesfam.insert({location, cesfam});
}

void System::addLocationsPatients(int location, Patient * patient) {
    if (this->locations_patients.find(location) == this->locations_patients.end()) {
        vector<Patient *> patients;
        this->locations_patients.insert({location, patients});
    }
    this->locations_patients.find(location)->second.push_back(patient);
}

void System::addLocationsManagers(int location, Manager * manager) {
    if (this->locations_managers.find(location) == this->locations_managers.end()) {
        vector<Manager *> managers;
        this->locations_managers.insert({location, managers});
    }
    this->locations_managers.find(location)->second.push_back(manager);
}

void System::initializeSystem(int cesfam_amount, int manager_amount, int patients_amount, string out_filename, string time) {
    printf("Entrando a initializeSystem\n");
    this->initializeCesfams(cesfam_amount);
    this->initializePatients(patients_amount, cesfam_amount);
    this->initializeManagers(manager_amount, cesfam_amount);

    // Se configura el monitor
    string output_str = "./out/out_" + out_filename + "_" + time + ".dat";
    Monitor *monitor = new Monitor(output_str);
    this->monitor = monitor;

    // Se disparan los primeros eventos en base a los pacientes existentes
    Event * e;
    int i = 0;
    printf("Cantidad de pacientes: %ld \n", this->patients.size());

    SimConfig * sim_config = SimConfig::getInstance("");
    double ASK_CONSENT_TIME = sim_config->getParams()["ask_consent_time"].get<double>();

    for (auto p = this->patients.begin(); p != this->patients.end(); ++p) {
        e = new Event(CallerType::AGENT_MANAGER, p->first, (ASK_CONSENT_TIME * i), ASK_CONSENT_TIME, 
                    ManagerEvents::ASK_CONSENT, p->second, p->second->getManager(), nullptr);
        printf("Evento: %d, puntero: %p, puntero manager: %p\n", i, e, p->second->getManager());
        this->event_list->insertEvent(e);
        i++;
    }
    printf("Saliendo de initilizeSystem\n");
}

void System::processEvent(Event * e) {
    delete e;
}

System::~System() {}
