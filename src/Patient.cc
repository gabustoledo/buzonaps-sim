#include "../includes/Patient.h"

int Patient::_curr_id(0);

Patient::Patient(EventList * _event_list, System * _system) : id(++_curr_id){
    this->event_list = _event_list;
    this->system = _system;
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

RiskCategories Patient::calcFinalRisk() {
    if (this->clinical_risk > this->social_risk) {
        return this->clinical_risk;
    }
    return this->social_risk;
}

void Patient::processEvent(Event * e) {
    printf("[PROCESS PATIENT EVENT - MANAGER %d - PATIENT %d] \n", this->manager->getId(), this->id);

    switch (e->getEventType())
    {
    case PatientEvents::ANSWER_CONSENT:
        printf("\t[PROCESS - ANSWER_CONSENT] \n");
        this->processAnswerConsent(e);
        break;

    case PatientEvents::RECEIVE_MEDICAL_HOUR:
        printf("\t[PROCESS - RECEIVE_MEDICAL_HOUR] \n");
        this->processReceiveMedicalHour(e);
        break;

    case PatientEvents::RECEIVE_TEST_HOUR:
        printf("\t[PROCESS - RECEIVE_TEST_HOUR] \n");
        this->processReceiveTestHour(e);
        break;

    case PatientEvents::RECEIVE_SOCIAL_HOUR:
        printf("\t[PROCESS - RECEIVE_SOCIAL_HOUR] \n");
        this->processReceiveSocialHour(e);
        break;

    case PatientEvents::RECEIVE_PSYCHO_HOUR:
        printf("\t[PROCESS - RECEIVE_PSYCHO_HOUR] \n");
        this->processReceivePsychoHour(e);
        break;
    
    case PatientEvents::ATTEND_MEDICAL_HOUR:
        printf("\t[PROCESS - ATTEND_MEDICAL_HOUR] \n");
        this->processAttendMedicalHour(e);
        break;

    case PatientEvents::ATTEND_TEST_HOUR:
        printf("\t[PROCESS - ATTEND_TEST_HOUR] \n");
        this->processAttendTestHour(e);
        break;

    case PatientEvents::ATTEND_SOCIAL_HOUR:
        printf("\t[PROCESS - ATTEND_SOCIAL_HOUR] \n");
        this->processAttendSocialHour(e);
        break;

    case PatientEvents::ATTEND_PSYCHO_HOUR:
        printf("\t[PROCESS - ATTEND_PSYCHO_HOUR] \n");
        this->processAttendPsychoHour(e);
        break;
    
    default:
        break;
    }
}

void Patient::processAnswerConsent(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double PRE_CLASSIFY_CLINICAL_RISK_TIME = sim_config->getParams()["pre_classify_clinical_risk_time"].get<double>();
    double ACCEPT_CONSENT_PROB = sim_config->getParams()["accept_consent_prob"].get<double>();

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
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ANSWER_CONSENT";
    this->system->log(log);
}

void Patient::processReceiveMedicalHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double ATTEND_MEDICAL_HOUR_PROB = sim_config->getParams()["attend_medical_hour_prob"].get<double>();
    double ATTEND_MEDICAL_HOUR_TIME = sim_config->getParams()["attend_medical_hour_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    double prob = dist(gen);
    if (prob < ATTEND_MEDICAL_HOUR_PROB) {
        printf("\t[INSERT - ATTEND_MEDICAL_HOUR] \n");
        Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), ATTEND_MEDICAL_HOUR_TIME, 
                                PatientEvents::ATTEND_MEDICAL_HOUR, this, this, nullptr);
        this->event_list->insertEvent(ev);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_MEDICAL_HOUR";
    this->system->log(log);
}

void Patient::processReceiveTestHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double ATTEND_TEST_HOUR_PROB = sim_config->getParams()["attend_test_hour_prob"].get<double>();
    double ATTEND_TEST_HOUR_TIME = sim_config->getParams()["attend_test_hour_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    double prob = dist(gen);
    if (prob < ATTEND_TEST_HOUR_PROB) {
        printf("\t[INSERT - ATTEND_TEST_HOUR] \n");
        Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), ATTEND_TEST_HOUR_TIME, 
                                PatientEvents::ATTEND_TEST_HOUR, this, this, nullptr);
        this->event_list->insertEvent(ev);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_TEST_HOUR";
    this->system->log(log);
}

void Patient::processReceiveSocialHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double ATTEND_SOCIAL_HOUR_PROB = sim_config->getParams()["attend_social_hour_prob"].get<double>();
    double ATTEND_SOCIAL_HOUR_TIME = sim_config->getParams()["attend_social_hour_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    double prob = dist(gen);
    if (prob < ATTEND_SOCIAL_HOUR_PROB) {
        printf("\t[INSERT - ATTEND_SOCIAL_HOUR] \n");
        Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), ATTEND_SOCIAL_HOUR_TIME, 
                                PatientEvents::ATTEND_SOCIAL_HOUR, this, this, nullptr);
        this->event_list->insertEvent(ev);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_SOCIAL_HOUR";
    this->system->log(log);
}

void Patient::processReceivePsychoHour(Event * e) {
    SimConfig * sim_config = SimConfig::getInstance("");
    double ATTEND_PSYCHO_HOUR_PROB = sim_config->getParams()["attend_psycho_hour_prob"].get<double>();
    double ATTEND_PSYCHO_HOUR_TIME = sim_config->getParams()["attend_psycho_hour_time"].get<double>();

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    double prob = dist(gen);
    if (prob < ATTEND_PSYCHO_HOUR_PROB) {
        printf("\t[INSERT - ATTEND_PSYCHO_HOUR] \n");
        Event * ev = new Event(CallerType::AGENT_PATIENT, this->getId(), e->getStartTime() + e->getExecTime(), ATTEND_PSYCHO_HOUR_TIME, 
                                PatientEvents::ATTEND_PSYCHO_HOUR, this, this, nullptr);
        this->event_list->insertEvent(ev);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_PSYCHO_HOUR";
    this->system->log(log);
}

void Patient::processAttendMedicalHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_MEDICAL_HOUR";
    this->system->log(log);
}

void Patient::processAttendTestHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_TEST_HOUR";
    this->system->log(log);
}

void Patient::processAttendSocialHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_SOCIAL_HOUR";
    this->system->log(log);
}

void Patient::processAttendPsychoHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_PSYCHO_HOUR";
    this->system->log(log);
}
