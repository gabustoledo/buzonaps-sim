#include "../includes/Patient.h"

int Patient::_curr_id(0);

Patient::Patient(RiskCategories _clinical_risk, RiskCategories _social_risk) : id(++_curr_id) {
    this->clinical_risk = _clinical_risk;
    this->social_risk = _social_risk;
}

Patient::Patient(EventList * _event_list) : id(++_curr_id){
    this->event_list = _event_list;
}

int Patient::getId() {
    return this->id;
}

Manager * Patient::getManager() {
    return this->manager;
}

void Patient::setManager(Manager * _manager) {
    this->manager = _manager;
}

void Patient::setClinicalRisk(RiskCategories _clinical_risk) {
    this->clinical_risk = _clinical_risk;
}

void Patient::setSocialRisk(RiskCategories _social_risk) {
    this->social_risk = _social_risk;
}

RiskCategories Patient::getClinicalRisk() {
    return this->clinical_risk;
}

RiskCategories Patient::getSocialRisk() {
    return this->social_risk;
}

void Patient::processEvent(Event * e) {
    printf("[PROCESS EVENT - PATIENT] \n");

    switch (e->getEventType())
    {
    case PatientEvents::ANSWER_CONSENT:
        printf("\t[PROCESS - ANSWER_CONSENT] \n");
        this->processAnswerConsent(e);
        break;
    
    default:
        break;
    }
}

void Patient::processAnswerConsent(Event * e) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double prob = dist(gen);
    if (prob < ACCEPT_CONSENT_PROB) {
        printf("\t[INSERT - PRE_CLASSIFY_CLINICAL_RISK] \n");
        Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
                            ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, this, this->getManager(), nullptr);
        this->event_list->insertEvent(ev);
    }
}
