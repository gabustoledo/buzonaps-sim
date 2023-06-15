#ifndef PATIENT_H
#define PATIENT_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Manager.h"

#define ANSWER_CONSENT_TIME 1
#define RECEIVE_MEDICAL_HOUR_TIME 1
#define RECEIVE_TEST_HOUR_TIME 1
#define RECEIVE_SOCIAL_HOUR_TIME 1
#define RECEIVE_PSYCHO_HOUR_TIME 1
#define ATTEND_MEDICAL_HOUR_TIME 1
#define ATTEND_TEST_HOUR_TIME 1
#define ATTEND_SOCIAL_HOUR_TIME 1
#define ATTEND_PSYCHO_HOUR_TIME 1

#define ACCEPT_CONSENT_PROB 0.5
#define ATTEND_MEDICAL_HOUR_PROB 0.5
#define ATTEND_TEST_HOUR_PROB 0.5
#define ATTEND_SOCIAL_HOUR_PROB 0.5
#define ATTEND_PSYCHO_HOUR_PROB 0.5

enum RiskCategories {
    HIGH,
    MEDIUM,
    LOW
};

enum PatientEvents {
    ANSWER_CONSENT,
    RECEIVE_MEDICAL_HOUR, 
    RECEIVE_TEST_HOUR,
    RECEIVE_SOCIAL_HOUR,
    RECEIVE_PSYCHO_HOUR,
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

    void processReceiveMedicalHour(Event * e);

    void processReceiveTestHour(Event * e);

    void processReceiveSocialHour(Event * e);

    void processReceivePsychoHour(Event * e);

    void processAttendMedicalHour(Event * e);

    void processAttendTestHour(Event * e);

    void processAttendSocialHour(Event * e);

    void processAttendPsychoHour(Event * e);

};

#endif // !PATIENT_H