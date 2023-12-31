#include "../includes/Manager.h"
#include <curl/curl.h>

int Manager::_curr_id(0);

Manager::Manager(Cesfam * _cesfam, EventList * _event_list, System * _system, int _location) : id(++_curr_id) {
    this->event_list = _event_list;
    this->system = _system;
    this->cesfam = _cesfam;
    this->location = _location;
}

int Manager::getId() {
    return this->id;
}

int Manager::getLocation() {
    return this->location;
}

void Manager::addPatient(Patient * patient) {
    this->patients.insert({patient, false});
}

map<Patient *, bool> Manager::getPatients() {
    return this->patients;
}

void Manager::processEvent(Event * e) {

    // printf("[PROCESS MANAGER EVENT - MANAGER %d - PATIENT %d] \n", this->id, e->getCallerId());

    // Si el evento ocurre fuera del horario laboral del gestor, se inserta nuevamente el evento al inicio de su jornada
    double out_of_time = this->outOfTime();
    if (out_of_time != 0) {
        // printf("\t[MANAGER OUT OF TIME - EVENT TYPE %d] \n", e->getEventType());
        double start_time = e->getStartTime() + out_of_time;
        Event * ev = new Event(e->getCallerType(), e->getCallerId(), start_time, e->getExecTime(), 
                                e->getEventType(), e->getCallerPtr(), e->getObjectivePtr(), nullptr);
        this->event_list->insertEvent(ev);
    }
    // Si ocurre dentro del horario laboral, se procesa el evento
    else {
        switch (e->getEventType()) {
        case ManagerEvents::ASK_CONSENT:
            // printf("\t[PROCESS - ASK_CONSENT] \n");
            this->processAskConsent(e);
            break;
        
        case ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK:
            // printf("\t[PROCESS - PRE_CLASSIFY_CLINICAL_RISK] \n");
            this->processPreClassifyClinicalRisk(e);
            break;

        case ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK:
            // printf("\t[PROCESS - PRE_CLASSIFY_SOCIAL_RISK] \n");
            this->processPreClassifySocialRisk(e);
            break;

        case ManagerEvents::MANAGE_PATIENT:
            // printf("\t[PROCESS - MANAGE_PATIENT] \n");
            this->processManagePatient(e);
            break;

        case ManagerEvents::MANAGE_MEDICAL_HOUR:
            // printf("\t[PROCESS - MANAGE_MEDICAL_HOUR] \n");d
            this->processMedicalHour(e);
            break;

        case ManagerEvents::MANAGE_TEST_HOUR:
            // printf("\t[PROCESS - MANAGE_TEST_HOUR] \n");
            this->processTestHour(e);
            break;

        case ManagerEvents::MANAGE_SOCIAL_HOUR:
            // printf("\t[PROCESS - MANAGE_SOCIAL_HOUR] \n");d
            this->processSocialHour(e);
            break;

        case ManagerEvents::MANAGE_PSYCHO_HOUR:
            // printf("\t[PROCESS - MANAGE_PSYCHO_HOUR] \n");
            this->processPsychoHour(e);
            break;
        
        case ManagerEvents::RE_EVALUATE_LOW_RISK:
            // printf("\t[PROCESS - RE_EVALUATE_LOW_RISK] \n");
            this->processReEvaluateLowRisk(e);
            break;

        case ManagerEvents::RE_EVALUATE_MANAGED:
            // printf("\t[PROCESS - RE_EVALUATE_MANAGED] \n");
            this->processReEvaluateManaged(e);
            break;

        default:
            break;
        }
    }
    delete e;
}

void Manager::processAskConsent(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double ANSWER_CONSENT_TIME = sim_config->getParams()["answer_consent_time"].get<double>();

    // Tarea que debe hacer el paciente
    // printf("\t[INSERT - ANSWER_CONSENT] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), ANSWER_CONSENT_TIME, 
                    PatientEvents::ANSWER_CONSENT, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ASK_CONSENT";
    this->system->log(log);
}

void Manager::processPreClassifyClinicalRisk(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double LOW_CLINICAL_RISK_PROB = sim_config->getParams()["low_clinical_risk_prob"].get<double>();
    double MEDIUM_CLINICAL_RISK_PROB = sim_config->getParams()["medium_clinical_risk_prob"].get<double>();
    // double PRE_CLASSIFY_SOCIAL_RISK_TIME = sim_config->getParams()["pre_classify_social_risk_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double prob = dist(gen);
    // printf("prob:  %f", prob);
    RiskCategories clinical_risk;
    if (prob < LOW_CLINICAL_RISK_PROB) {
        clinical_risk = RiskCategories::LOW;
    } else if (prob >= LOW_CLINICAL_RISK_PROB && prob < (LOW_CLINICAL_RISK_PROB + MEDIUM_CLINICAL_RISK_PROB)) {
        clinical_risk = RiskCategories::MEDIUM;
    } else {
        clinical_risk = RiskCategories::HIGH;
    }


    if (e->getCallerType() == CallerType::AGENT_PATIENT) {
        Patient * patient = (Patient *)e->getCallerPtr();
        // Si no tiene un riesgo asociado, se asigna uno aleatorio
        if (patient->getClinicalRisk() == RiskCategories::UNDEFINED) {
            patient->setClinicalRisk(clinical_risk);
        } 
        patient->setClinicalRisk(clinical_risk);

        std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
        std::string id_manager_str = std::to_string(this->getId());
        std::string process_str = "1";

        std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

        CURL *curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

            curl_easy_cleanup(curl);
        }

        // Tarea que debe hacer el manager
        //TODO: Registrar el resultado del riesgo clínico
        // printf("\t[INSERT - PRE_CLASSIFY_SOCIAL_RISK] \n");
        // Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_SOCIAL_RISK_TIME, 
        //                     ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
        //this->event_list->insertEvent(ev);
    }
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "PRE_CLASSIFY_CLINICAL_RISK";
    this->system->log(log);
}

void Manager::processPreClassifySocialRisk(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double LOW_SOCIAL_RISK_PROB = sim_config->getParams()["low_social_risk_prob"].get<double>();
    double MEDIUM_SOCIAL_RISK_PROB = sim_config->getParams()["medium_social_risk_prob"].get<double>();
    // double MANAGE_PATIENT_TIME = sim_config->getParams()["manage_patient_time"].get<double>();
    // double RE_EVALUATE_LOW_RISK_TIME = sim_config->getParams()["re_evaluate_low_risk_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double prob = dist(gen);
    // printf("prob:  %f", prob);
    RiskCategories social_risk;
    if (prob < LOW_SOCIAL_RISK_PROB) {
        social_risk = RiskCategories::LOW;
    } else if (prob >= LOW_SOCIAL_RISK_PROB && prob < (LOW_SOCIAL_RISK_PROB + MEDIUM_SOCIAL_RISK_PROB)) {
        social_risk = RiskCategories::MEDIUM;
    } else {
        social_risk = RiskCategories::HIGH;
    }

    if (e->getCallerType() == CallerType::AGENT_PATIENT) {
        Patient * patient = (Patient *)e->getCallerPtr();
        // Si no tiene un riesgo asociado, se asigna uno aleatorio
        if (patient->getSocialRisk() == RiskCategories::UNDEFINED) {
            patient->setSocialRisk(social_risk); 
        }
        //TODO: Registrar el resultado del riesgo social
        RiskCategories final_risk = patient->calcFinalRisk();
        if (final_risk == RiskCategories::MEDIUM || final_risk == RiskCategories::HIGH) {

            std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
            std::string id_manager_str = std::to_string(this->getId());
            std::string process_str = "2";

            std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

            CURL *curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

                CURLcode res = curl_easy_perform(curl);
                if(res != CURLE_OK)
                    std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

                curl_easy_cleanup(curl);
            }
            // Tarea que debe hacer el manager
            // printf("\t[INSERT - MANAGE_PATIENT] \n");d
            // Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_PATIENT_TIME, 
            //                     ManagerEvents::MANAGE_PATIENT, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
            // this->event_list->insertEvent(ev);
        } else {

            std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
            std::string id_manager_str = std::to_string(this->getId());
            std::string process_str = "7";

            std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

            CURL *curl = curl_easy_init();
            if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

                CURLcode res = curl_easy_perform(curl);
                if(res != CURLE_OK)
                    std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

                curl_easy_cleanup(curl);
            }
            // Tarea que debe hacer el manager
            // printf("\t[INSERT - RE_EVALUATE_LOW_RISK] \n");
            // Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), RE_EVALUATE_LOW_RISK_TIME, 
            //                     ManagerEvents::RE_EVALUATE_LOW_RISK, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
            // this->event_list->insertEvent(ev);
        }
        //TODO: Registrar el resultado del riesgo final
    }
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "PRE_CLASSIFY_SOCIAL_RISK";
    this->system->log(log);
}

void Manager::processReEvaluateLowRisk(Event * e) {
    // SimConfig * sim_config = SimConfig::getInstance("");
    // double PRE_CLASSIFY_CLINICAL_RISK_TIME = sim_config->getParams()["pre_classify_clinical_risk_time"].get<double>();

    std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
    std::string id_manager_str = std::to_string(this->getId());
    std::string process_str = "0";

    std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    // Tarea que debe hacer el manager
    // printf("\t[INSERT - PRE_CLASSIFY_CLINICAL_RISK] \n");d
    // Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
    //                         ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, e->getCallerPtr(), this, nullptr);
    // this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RE_EVALUATE_LOW_RISK";
    this->system->log(log);
}

void Manager::processReEvaluateManaged(Event * e) {
    // SimConfig * sim_config = SimConfig::getInstance("");
    // double PRE_CLASSIFY_CLINICAL_RISK_TIME = sim_config->getParams()["pre_classify_clinical_risk_time"].get<double>();

    std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
    std::string id_manager_str = std::to_string(this->getId());
    std::string process_str = "0";

    std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    // Tarea que debe hacer el manager
    // printf("\t[INSERT - PRE_CLASSIFY_CLINICAL_RISK] \n");
    // Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
    //                         ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, e->getCallerPtr(), this, nullptr);
    // this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RE_EVALUATE_MANAGED";
    this->system->log(log);
}

void Manager::processManagePatient(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double MEDICAL_HOUR_PROB = sim_config->getParams()["medical_hour_prob"].get<double>();
    double TEST_HOUR_PROB = sim_config->getParams()["test_hour_prob"].get<double>();
    double SOCIAL_HOUR_PROB = sim_config->getParams()["social_hour_prob"].get<double>();
    double PSYCHO_HOUR_PROB = sim_config->getParams()["psycho_hour_prob"].get<double>();
    // double MANAGE_MEDICAL_HOUR_TIME = sim_config->getParams()["manage_medical_hour_time"].get<double>();
    // double MANAGE_TEST_HOUR_TIME = sim_config->getParams()["manage_test_hour_time"].get<double>();
    // double MANAGE_SOCIAL_HOUR_TIME = sim_config->getParams()["manage_social_hour_time"].get<double>();
    // double MANAGE_PSYCHO_HOUR_TIME = sim_config->getParams()["manage_psycho_hour_time"].get<double>();
    // double RE_EVALUATE_MANAGED_TIME = sim_config->getParams()["re_evaluate_managed_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    // Event * ev;

    // Se gestionan las horas médicas
    double medical_prob = dist(gen);
    if (medical_prob < MEDICAL_HOUR_PROB) {

        std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
        std::string id_manager_str = std::to_string(this->getId());
        std::string process_str = "3";

        std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

        CURL *curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

            curl_easy_cleanup(curl);
        }
        // Tarea que debe hacer el manager
        // printf("\t[INSERT - MANAGE_MEDICAL_HOUR] \n");
        // ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_MEDICAL_HOUR_TIME, 
        //                         ManagerEvents::MANAGE_MEDICAL_HOUR, e->getCallerPtr(), this, nullptr);
        // this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas a exámenes
    double test_prob = dist(gen);
    if (test_prob < TEST_HOUR_PROB) {
        

        std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
        std::string id_manager_str = std::to_string(this->getId());
        std::string process_str = "4";

        std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

        CURL *curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

            curl_easy_cleanup(curl);
        }
        // Tarea que debe hacer el manager
        // printf("\t[INSERT - MANAGE_TEST_HOUR] \n");
        // ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_TEST_HOUR_TIME, 
        //                         ManagerEvents::MANAGE_TEST_HOUR, e->getCallerPtr(), this, nullptr);
        // this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas con asistente social
    double social_prob = dist(gen);
    if (social_prob < SOCIAL_HOUR_PROB) {

        std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
        std::string id_manager_str = std::to_string(this->getId());
        std::string process_str = "5";

        std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

        CURL *curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

            curl_easy_cleanup(curl);
        }
        // Tarea que debe hacer el manager
        // printf("\t[INSERT - MANAGE_SOCIAL_HOUR] \n");
        // ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_SOCIAL_HOUR_TIME, 
        //                         ManagerEvents::MANAGE_SOCIAL_HOUR, e->getCallerPtr(), this, nullptr);
        // this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas para atención psicológica
    double psycho_prob = dist(gen);
    if (psycho_prob < PSYCHO_HOUR_PROB) {

        std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
        std::string id_manager_str = std::to_string(this->getId());
        std::string process_str = "6";

        std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

        CURL *curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK)
                std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

            curl_easy_cleanup(curl);
        }
        // Tarea que debe hacer el manager
        // printf("\t[INSERT - MANAGE_PSYCHO_HOUR] \n");
        // ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_PSYCHO_HOUR_TIME, 
        //                         ManagerEvents::MANAGE_PSYCHO_HOUR, e->getCallerPtr(), this, nullptr);
        // this->event_list->insertEvent(ev);
    }

    std::string id_patient_str = std::to_string(((Patient *)e->getCallerPtr())->getId());
    std::string id_manager_str = std::to_string(this->getId());
    std::string process_str = "8";

    std::string url = "http://localhost:3000/api/sim/pendiente/" + id_patient_str + "/" + id_manager_str + "/" + process_str;

    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    // Tarea que debe hacer el manager
    // Se inserta el evento de reevaluar el riesgo
    // printf("\t[INSERT - RE_EVALUATE_MANAGED] \n");
    // ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), RE_EVALUATE_MANAGED_TIME, 
    //                         ManagerEvents::RE_EVALUATE_MANAGED, e->getCallerPtr(), this, nullptr);
    // this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "MANAGE_PATIENT";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    bool managed_state = ((Patient *)e->getCallerPtr())->getManagedState();
    if (!managed_state) {
        stats->updateManagedPatientsAmount();
        ((Patient *)e->getCallerPtr())->setManagedState(true);
    }
    stats->updateTotalInterventions();
}

void Manager::processMedicalHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double RECEIVE_MEDICAL_HOUR_TIME = sim_config->getParams()["receive_medical_hour_time"].get<double>();

    // Tarea que debe hacer el paciente
    // printf("\t[INSERT - RECEIVE_MEDICAL_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_MEDICAL_HOUR_TIME, 
                            PatientEvents::RECEIVE_MEDICAL_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "MANAGE_MEDICAL_HOUR";
    this->system->log(log);
}

void Manager::processTestHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double RECEIVE_TEST_HOUR_TIME = sim_config->getParams()["receive_test_hour_time"].get<double>();

    // Tarea que debe hacer el paciente
    // printf("\t[INSERT - RECEIVE_TEST_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_TEST_HOUR_TIME, 
                            PatientEvents::RECEIVE_TEST_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "MANAGE_TEST_HOUR";
    this->system->log(log);
}

void Manager::processSocialHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double RECEIVE_SOCIAL_HOUR_TIME = sim_config->getParams()["receive_social_hour_time"].get<double>();

    // Tarea que debe hacer el paciente
    // printf("\t[INSERT - RECEIVE_SOCIAL_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_SOCIAL_HOUR_TIME, 
                            PatientEvents::RECEIVE_SOCIAL_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "MANAGE_SOCIAL_HOUR";
    this->system->log(log);
}

void Manager::processPsychoHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double RECEIVE_PSYCHO_HOUR_TIME = sim_config->getParams()["receive_psycho_hour_time"].get<double>();

    // Tarea que debe hacer el paciente
    // printf("\t[INSERT - RECEIVE_PSYCHO_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_PSYCHO_HOUR_TIME, 
                            PatientEvents::RECEIVE_PSYCHO_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
    json log;
    log["agent_type"] = "MANAGER";
    log["agent_id"] = this->id;
    log["manager_id"] = this->id;
    log["patient_id"] = ((Patient *)e->getCallerPtr())->getId();
    log["clinical_risk"] = "";
    log["social_risk"] = "";
    log["final_risk"] = "";
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "MANAGE_PSYCHO_HOUR";
    this->system->log(log);
}

double Manager::outOfTime() {
    SimConfig * sim_config = SimConfig::getInstance("");
    int MANAGER_START_HOUR = sim_config->getParams()["manager_start_hour"].get<int>();
    int MANAGER_END_HOUR = sim_config->getParams()["manager_end_hour"].get<int>();

    int curr_clock = (int)this->event_list->getClock();
    // Se calcula en base a las 24 horas del día
    int curr_hour = curr_clock % 24;

    if (curr_hour >= MANAGER_START_HOUR && curr_hour <= MANAGER_END_HOUR) {
        return 0;
    } else if (curr_hour < MANAGER_START_HOUR) {
        return MANAGER_START_HOUR - curr_hour;
    } else if (curr_hour > MANAGER_END_HOUR) {
        return 24 - curr_hour + MANAGER_START_HOUR;
    }
    return -1;
}
