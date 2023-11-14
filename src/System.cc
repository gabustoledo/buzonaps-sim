#include "../includes/System.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <json.hpp>
#include <curl/curl.h>

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
    using Tupla = std::tuple<int, int>;
    std::vector<Tupla> tuplas;
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

                int manager_id = this->locations_managers.find(it->first)->second[p]->getId();
                int patient_id = this->locations_patients.find(it->first)->second[j]->getId();
                tuplas.push_back(std::make_tuple(manager_id, patient_id));
            }
            k = j;
        }
    }

    // Crear un objeto JSON
    nlohmann::json json_obj;
    for (const auto& t : tuplas) {
        json_obj.push_back({{"manager", std::get<0>(t)}, {"patient", std::get<1>(t)}});
    }

    // Iniciar CURL
    std::string jsonStr = json_obj.dump();
    curl_global_init(CURL_GLOBAL_ALL);
    std::string url = "http://localhost:3000/api/sim/managerPatients";
    CURL *curl = curl_easy_init();
    if(curl) {

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Configurar la petición POST
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Establece el método de solicitud como POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Establece los datos JSON en el cuerpo de la solicitud
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

        curl_easy_perform(curl);

        // Limpiar
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

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
