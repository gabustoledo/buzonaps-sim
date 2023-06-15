#ifndef PATIENT_H
#define PATIENT_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Manager.h"

#define ACCEPT_CONSENT_PROB 0.5
#define ANSWER_CONSENT_TIME 1

enum RiskCategories {
    HIGH,
    MEDIUM,
    LOW
};

enum PatientEvents {
    ANSWER_CONSENT,
    ATTEND_MEDICAL_HOUR, 
    ATTEND_TEST_HOUR,
    ATTEND_SOCIAL_HOUR,
    ATTEND_PSYCHO_HOUR
};

class Manager;

class Patient : public Agent {

private:
    static int _curr_id;
    int id;
    RiskCategories clinical_risk;
    RiskCategories social_risk;
    Cesfam * cesfam;
    Manager * manager;

public:
    Patient(RiskCategories _clinical_risk, RiskCategories _social_risk);

    Patient(EventList * _event_list);

    int getId();

    Manager * getManager();

    void setManager(Manager * _manager);

    void setClinicalRisk(RiskCategories _clinical_risk);

    void setSocialRisk(RiskCategories _clinical_social);

    RiskCategories getClinicalRisk();

    RiskCategories getSocialRisk();

    void processEvent(Event * e) override;

    void processAnswerConsent(Event * e);

};

#endif // !PATIENT_H