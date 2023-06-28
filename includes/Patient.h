#ifndef PATIENT_H
#define PATIENT_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Manager.h"
#include "System.h"
#include "SimConfig.h"

enum RiskCategories {
    HIGH = 30,
    MEDIUM = 20,
    LOW = 10,
    UNDEFINED = 0
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

enum HoursTypes {
    MEDICAL_HOUR, 
    TEST_HOUR,
    SOCIAL_HOUR, 
    PSYCHO_HOUR
};



class Manager;

class Patient : public Agent {

private:
    static int _curr_id;
    int id;
    RiskCategories clinical_risk = RiskCategories::UNDEFINED;
    RiskCategories social_risk = RiskCategories::UNDEFINED;
    Cesfam * cesfam;
    Manager * manager;
    map<HoursTypes, int> hours_attended;

    RiskCategories classifyIntRisk(int value);
    void setHoursAttended(HoursTypes type, int amount);

public:
    Patient(EventList * _event_list, System * system);

    int getId();

    Manager * getManager();

    void setManager(Manager * _manager);

    void setClinicalRisk(RiskCategories _clinical_risk);

    void setSocialRisk(RiskCategories _clinical_social);

    RiskCategories getClinicalRisk();

    RiskCategories getSocialRisk();

    RiskCategories getFinalRisk();

    RiskCategories calcFinalRisk();

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