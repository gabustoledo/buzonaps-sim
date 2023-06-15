#include "../includes/Manager.h"

int Manager::_curr_id(0);

Manager::Manager(Cesfam * _cesfam, vector<Patient *> _patients, EventList * _event_list){
    this->event_list = _event_list;
    this->cesfam = _cesfam;
    size_t i;
    for (i = 0; i < _patients.size(); i++) {
        this->patients.insert({_patients[i], false});
    }
}

int Manager::getId() {
    return this->id;
}

void Manager::processEvent(Event * e) {

    printf("[PROCESS MANAGER EVENT - MANAGER %d - PATIENT %d] \n", this->id, e->getCallerId());

    switch (e->getEventType()) {
    case ManagerEvents::ASK_CONSENT:
        printf("\t[PROCESS - ASK_CONSENT] \n");
        this->processAskConsent(e);
        break;
    
    case ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK:
        printf("\t[PROCESS - PRE_CLASSIFY_CLINICAL_RISK] \n");
        this->processPreClassifyClinicalRisk(e);
        break;

    case ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK:
        printf("\t[PROCESS - PRE_CLASSIFY_SOCIAL_RISK] \n");
        this->processPreClassifySocialRisk(e);
        break;

    case ManagerEvents::MANAGE_PATIENT:
        printf("\t[PROCESS - MANAGE_PATIENT] \n");
        this->processManagePatient(e);
        break;

    case ManagerEvents::MANAGE_MEDICAL_HOUR:
        printf("\t[PROCESS - MANAGE_MEDICAL_HOUR] \n");
        this->processMedicalHour(e);
        break;

    case ManagerEvents::MANAGE_TEST_HOUR:
        printf("\t[PROCESS - MANAGE_TEST_HOUR] \n");
        this->processTestHour(e);
        break;

    case ManagerEvents::MANAGE_SOCIAL_HOUR:
        printf("\t[PROCESS - MANAGE_SOCIAL_HOUR] \n");
        this->processSocialHour(e);
        break;

    case ManagerEvents::MANAGE_PSYCHO_HOUR:
        printf("\t[PROCESS - MANAGE_PSYCHO_HOUR] \n");
        this->processPsychoHour(e);
        break;
    
    case ManagerEvents::RE_EVALUATE_LOW_RISK:
        printf("\t[PROCESS - RE_EVALUATE_LOW_RISK] \n");
        this->processReEvaluateLowRisk(e);
        break;

    case ManagerEvents::RE_EVALUATE_MANAGED:
        printf("\t[PROCESS - RE_EVALUATE_MANAGED] \n");
        this->processReEvaluateManaged(e);
        break;

    default:
        break;
    }
}

void Manager::processAskConsent(Event * e) {
    printf("\t[INSERT - ANSWER_CONSENT] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), ANSWER_CONSENT_TIME, 
                    PatientEvents::ANSWER_CONSENT, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processPreClassifyClinicalRisk(Event * e) {
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
        patient->setClinicalRisk(clinical_risk); 

        if (clinical_risk == RiskCategories::MEDIUM || clinical_risk == RiskCategories::HIGH) {
            printf("\t[INSERT - PRE_CLASSIFY_SOCIAL_RISK] \n");
            Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_SOCIAL_RISK_TIME, 
                                ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
            this->event_list->insertEvent(ev);
        } else if (this->patients.find(patient) != this->patients.end() && !this->patients.find(patient)->second) {
            // Si es primera vez que el paciente es evaluado y obtiene un riesgo bajo, se reevalua en un tiempo determinado
            printf("\t[INSERT - RE_EVALUATE_LOW_RISK] \n");
            Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), RE_EVALUATE_LOW_RISK_TIME, 
                                ManagerEvents::RE_EVALUATE_LOW_RISK, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
            this->event_list->insertEvent(ev);
            // Se marca al paciente como evaluado
            if (this->patients.find(patient) != this->patients.end()) {
                this->patients.find(patient)->second = true;
            }
            //TODO: Registrar el resultado del riesgo en la segunda evaluación
        }
    }
}

void Manager::processPreClassifySocialRisk(Event * e) {
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
        patient->setSocialRisk(social_risk); 
        // Si el paciente ya fue evaluado antes, no se gestionará
        if (this->patients.find(patient) != this->patients.end() && !this->patients.find(patient)->second) {
            if (social_risk == RiskCategories::MEDIUM || social_risk == RiskCategories::HIGH) {
                printf("\t[INSERT - MANAGE_PATIENT] \n");
                Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_PATIENT_TIME, 
                                    ManagerEvents::MANAGE_PATIENT, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
                this->event_list->insertEvent(ev);
            }
            // Independiente del riesgo que tenga, se marca como evaluado
            this->patients.find(patient)->second = true; //?
        }
        //TODO: Registrar el resultado del riesgo en la segunda evaluación
    }
}

void Manager::processReEvaluateLowRisk(Event * e) {
    printf("\t[INSERT - PRE_CLASSIFY_CLINICAL_RISK] \n");
    Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
                            ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, e->getCallerPtr(), this, nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processReEvaluateManaged(Event * e) {
    printf("\t[INSERT - PRE_CLASSIFY_CLINICAL_RISK] \n");
    Event * ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
                            ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, e->getCallerPtr(), this, nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processManagePatient(Event * e) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    Event * ev;

    // Se gestionan las horas médicas
    double medical_prob = dist(gen);
    if (medical_prob < MEDICAL_HOUR_PROB) {
        printf("\t[INSERT - MANAGE_MEDICAL_HOUR] \n");
        ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_MEDICAL_HOUR_TIME, 
                                ManagerEvents::MANAGE_MEDICAL_HOUR, e->getCallerPtr(), this, nullptr);
        this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas a exámenes
    double test_prob = dist(gen);
    if (test_prob < TEST_HOUR_PROB) {
        printf("\t[INSERT - MANAGE_TEST_HOUR] \n");
        ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_TEST_HOUR_TIME, 
                                ManagerEvents::MANAGE_TEST_HOUR, e->getCallerPtr(), this, nullptr);
        this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas con asistente social
    double social_prob = dist(gen);
    if (social_prob < SOCIAL_HOUR_PROB) {
        printf("\t[INSERT - MANAGE_SOCIAL_HOUR] \n");
        ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_SOCIAL_HOUR_TIME, 
                                ManagerEvents::MANAGE_SOCIAL_HOUR, e->getCallerPtr(), this, nullptr);
        this->event_list->insertEvent(ev);
    }

    // Se gestionan las horas para atención psicológica
    double psycho_prob = dist(gen);
    if (psycho_prob < PSYCHO_HOUR_PROB) {
        printf("\t[INSERT - MANAGE_PSYCHO_HOUR] \n");
        ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), MANAGE_PSYCHO_HOUR_TIME, 
                                ManagerEvents::MANAGE_PSYCHO_HOUR, e->getCallerPtr(), this, nullptr);
        this->event_list->insertEvent(ev);
    }

    // Se inserta el evento de reevaluar el riesgo
    printf("\t[INSERT - MANAGE_PSYCHO_HOUR] \n");
    ev = new Event(CallerType::AGENT_PATIENT, ((Patient *)e->getCallerPtr())->getId(), e->getStartTime() + e->getExecTime(), RE_EVALUATE_MANAGED_TIME, 
                            ManagerEvents::RE_EVALUATE_MANAGED, e->getCallerPtr(), this, nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processMedicalHour(Event * e) {
    printf("\t[INSERT - RECEIVE_MEDICAL_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_MEDICAL_HOUR_TIME, 
                            PatientEvents::RECEIVE_MEDICAL_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processTestHour(Event * e) {
    printf("\t[INSERT - RECEIVE_TEST_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_TEST_HOUR_TIME, 
                            PatientEvents::RECEIVE_TEST_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processSocialHour(Event * e) {
    printf("\t[INSERT - RECEIVE_SOCIAL_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_SOCIAL_HOUR_TIME, 
                            PatientEvents::RECEIVE_SOCIAL_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processPsychoHour(Event * e) {
    printf("\t[INSERT - RECEIVE_PSYCHO_HOUR] \n");
    Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RECEIVE_PSYCHO_HOUR_TIME, 
                            PatientEvents::RECEIVE_PSYCHO_HOUR, this, e->getCallerPtr(), nullptr);
    this->event_list->insertEvent(ev);
}
