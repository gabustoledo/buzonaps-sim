#include "../includes/Patient.h"

int Patient::_curr_id(0);

Patient::Patient(EventList * _event_list, System * _system, int _location) : id(++_curr_id){
    this->event_list = _event_list;
    this->system = _system;
    this->location = _location;
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

void Patient::setManagedState(bool value) {
    this->managed_state = value;
}

bool Patient::getManagedState() {
    return this->managed_state;
}

int Patient::getLocation() {
    return this->location;
}

void Patient::setClinicalRisk(RiskCategories _clinical_risk) {
    this->clinical_risk = _clinical_risk;
}

void Patient::setSocialRisk(RiskCategories _social_risk) {
    this->social_risk = _social_risk;
}

// La idea de este método es que el paciente acumule (o pierda) "puntaje" para disminuir (o aumentar) su riesgo
void Patient::setHoursAttended(HoursTypes type, int amount) {
    // Si no existe el tipo de hora en el paciente, se inserta con su cantidad (positiva o negativa)
    if (this->hours_attended.find(type) == this->hours_attended.end()) {
        this->hours_attended.insert({type, amount});
    } else {
        // Si existe, se actualiza el "puntaje" asociado al tipo de hora
        this->hours_attended.find(type)->second += amount;
    }
}

RiskCategories Patient::getClinicalRisk() {
    return this->clinical_risk;
}

RiskCategories Patient::getSocialRisk() {
    return this->social_risk;
}

RiskCategories Patient::getFinalRisk() {
    if (this->clinical_risk > this->social_risk) {
        return this->clinical_risk;
    }
    return this->social_risk;
}

RiskCategories Patient::classifyIntRisk(int value) {
    RiskCategories result;
    if (value >= RiskCategories::HIGH) {
        result = RiskCategories::HIGH;
    } else if (value < RiskCategories::HIGH && value >= RiskCategories::MEDIUM) {
        result = RiskCategories::MEDIUM;
    } else {
        result = RiskCategories::LOW;
    }
    return result;
}

// Se calcula el riesgo en base a las horas que tiene
RiskCategories Patient::calcFinalRisk() {
    RiskCategories actual_risk = this->getFinalRisk();
    SimConfig * sim_config = SimConfig::getInstance("");
    int MEDICAL_HOUR_FACTOR = sim_config->getParams()["medical_hour_factor"].get<int>();
    int TEST_HOUR_FACTOR = sim_config->getParams()["test_hour_factor"].get<int>();
    int SOCIAL_HOUR_FACTOR = sim_config->getParams()["social_hour_factor"].get<int>();
    int PSYCHO_HOUR_FACTOR = sim_config->getParams()["psycho_hour_factor"].get<int>();
    int clinical_r = this->clinical_risk;
    int social_r = this->social_risk;
    // Si el mapa de horas agendadas tiene algún dato, se puede modificar el riesgo según sus horas agendadas considerando a las que asistió y a las que no
    if (!this->hours_attended.empty()) {
        if (this->hours_attended.find(HoursTypes::MEDICAL_HOUR) != this->hours_attended.end()) {
            // Se consindera el "puntaje" asociado a las horas médicas para recalcular el riesgo clínico
            int medical = this->hours_attended.find(HoursTypes::MEDICAL_HOUR)->second * MEDICAL_HOUR_FACTOR;
            clinical_r += (-1 * medical);
        } 
        if (this->hours_attended.find(HoursTypes::TEST_HOUR) != this->hours_attended.end()) {
            // Se consindera el "puntaje" asociado a las horas médicas para recalcular el riesgo clínico
            int test = this->hours_attended.find(HoursTypes::TEST_HOUR)->second * TEST_HOUR_FACTOR;
            clinical_r += (-1 * test);
        } 
        // Se modifica el riesgo como enum de riesgos para riesgo clínico
        this->clinical_risk = classifyIntRisk(clinical_r);
        if (this->hours_attended.find(HoursTypes::SOCIAL_HOUR) != this->hours_attended.end()) {
            // Se consindera el "puntaje" asociado a las horas médicas para recalcular el riesgo social
            int social = this->hours_attended.find(HoursTypes::SOCIAL_HOUR)->second * SOCIAL_HOUR_FACTOR;
            social_r += (-1 * social);
        }
        if (this->hours_attended.find(HoursTypes::PSYCHO_HOUR) != this->hours_attended.end()) {
            // Se consindera el "puntaje" asociado a las horas médicas para recalcular el riesgo social
            int psycho = this->hours_attended.find(HoursTypes::PSYCHO_HOUR)->second * PSYCHO_HOUR_FACTOR;
            social_r += (-1 * psycho);
        }
        // Se modifica el riesgo como enum de riesgos para riesgo social
        this->social_risk = classifyIntRisk(social_r);
    }

    RiskCategories new_risk;
    // Se define el riesgo final como el mayor entre ambos (clínico y social)
    if (this->clinical_risk > this->social_risk) {
        new_risk = this->clinical_risk;
    }
    new_risk = this->social_risk;
    
    if (actual_risk > new_risk) {
        Stats * stats = Stats::getInstance();
        stats->updateDecreasedRiskEvents();
    } else if (actual_risk < new_risk) {
        Stats * stats = Stats::getInstance();
        stats->updateIncreasedRiskEvents();
    }

    return new_risk;
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
    delete e;
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
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
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
        // Si va, se agrega a las horas a las que ha asisitido con valor 1
        setHoursAttended(HoursTypes::MEDICAL_HOUR, 1);
    } else {
        // Si no va, se agrega a las horas a las que ha asistido con valor -1
        setHoursAttended(HoursTypes::MEDICAL_HOUR, -1);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_MEDICAL_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalMedicalHourReceived();
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
        this->event_list->insertEvent(ev);// Si va, se agrega a las horas a las que ha asisitido con valor 1
        setHoursAttended(HoursTypes::TEST_HOUR, 1);
    } else {
        // Si no va, se agrega a las horas a las que ha asistido con valor -1
        setHoursAttended(HoursTypes::TEST_HOUR, -1);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_TEST_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalTestHourReceived();
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
        this->event_list->insertEvent(ev);// Si va, se agrega a las horas a las que ha asisitido con valor 1
        setHoursAttended(HoursTypes::SOCIAL_HOUR, 1);
    } else {
        // Si no va, se agrega a las horas a las que ha asistido con valor -1
        setHoursAttended(HoursTypes::SOCIAL_HOUR, -1);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_SOCIAL_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalSocialHourReceived();
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
        this->event_list->insertEvent(ev);// Si va, se agrega a las horas a las que ha asisitido con valor 1
        setHoursAttended(HoursTypes::PSYCHO_HOUR, 1);
    } else {
        // Si no va, se agrega a las horas a las que ha asistido con valor -1
        setHoursAttended(HoursTypes::PSYCHO_HOUR, -1);
    }
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "RECEIVE_PSYCHO_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalPsychoHourReceived();
}

void Patient::processAttendMedicalHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_MEDICAL_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalMedicalHourAttended();
}

void Patient::processAttendTestHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_TEST_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalTestHourAttended();
}

void Patient::processAttendSocialHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_SOCIAL_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalSocialHourAttended();
}

void Patient::processAttendPsychoHour(Event * e) {
    json log;
    log["agent_type"] = "PATIENT";
    log["agent_id"] = this->id;
    log["manager_id"] = this->getManager()->getId();
    log["patient_id"] = this->id;
    log["clinical_risk"] = this->clinical_risk;
    log["social_risk"] = this->social_risk;
    log["final_risk"] = this->getFinalRisk();
    log["event_id"] = e->getId();
    log["sim_clock"] = this->event_list->getClock();
    log["process"] = "ATTEND_PSYCHO_HOUR";
    this->system->log(log);

    Stats * stats = Stats::getInstance();
    stats->updateTotalPsychoHourAttended();
}
