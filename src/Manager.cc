#include "../includes/Manager.h"

int Manager::_curr_id(0);

Manager::Manager(Cesfam * _cesfam, vector<Patient *> _patients, EventList * _event_list) {
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

    printf("[PROCESS EVENT - MANAGER] \n");
    Event * ev;

    switch (e->getEventType()) {
    case ManagerEvents::ASK_CONSENT:
        printf("\t[PROCESS - ASK_CONSENT] \n");
        printf("\t[INSERT - ANSWER_CONSENT] \n");
        ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), ANSWER_CONSENT_TIME, 
                        PatientEvents::ANSWER_CONSENT, this, e->getCallerPtr(), nullptr);
        this->event_list->insertEvent(ev);
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

        break;

    case ManagerEvents::MANAGE_TEST_HOUR:
        printf("\t[PROCESS - MANAGE_TEST_HOUR] \n");

        break;
    
    case ManagerEvents::RE_EVALUATE_LOW_RISK:
        printf("\t[PROCESS - RE_EVALUATE_LOW_RISK] \n");

        break;

    case ManagerEvents::RE_EVALUATE_MANAGED:
        printf("\t[PROCESS - RE_EVALUATE_MANAGED] \n");
        break;

    default:
        break;
    }
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
            Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_SOCIAL_RISK_TIME, 
                                ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK, e->getCallerPtr(), this, nullptr); // Se determina como agente objetivo el mismo gestor dado que es este quien procesa el siguiente evento
            this->event_list->insertEvent(ev);
        } else if (this->patients.find(patient) != this->patients.end() && !this->patients.find(patient)->second) {
            // Si es primera vez que el paciente es evaluado y obtiene un riesgo bajo, se reevalua en un tiempo determinado
            printf("\t[INSERT - RE_EVALUATE_LOW_RISK] \n");
            Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), RE_EVALUATE_LOW_RISK_TIME, 
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
                Event * ev = new Event(CallerType::AGENT_MANAGER, this->getId(), e->getStartTime() + e->getExecTime(), MANAGE_PATIENT_TIME, 
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
    Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
                            ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, e->getCallerPtr(), this, nullptr);
    this->event_list->insertEvent(ev);
}

void Manager::processManagePatient(Event * e) {
    // Se gestionan las horas médicas

    // Se gestionan las horas a exámenes

    // Se gestionan las horas con asistente social

    // Se gestionan las horas para atención psicológica
}