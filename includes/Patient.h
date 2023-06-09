#ifndef PATIENT_H
#define PATIENT_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"

enum RiskCategories {
    HIGH,
    MEDIUM,
    LOW
};

enum PatientEvents {
    ATTEND_MEDICAL_HOUR, 
    ATTEND_TEST_HOUR,
    ATTEND_SOCIAL_HOUR,
    ATTEND_PSYCHO_HOUR
};

class Patient : public Agent {

private:
    static int _curr_id;
    int id;
    RiskCategories clinical_risk;
    RiskCategories social_risk;
    Cesfam * cesfam;

public:
    Patient(RiskCategories _clinical_risk, RiskCategories _social_risk);

    Patient();

    void processEvent(Event * e);

    int getId();
};

#endif // !PATIENT_H