#ifndef MANAGER_H
#define MANAGER_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Patient.h"

#define ASK_CONSENT_TIME 1 // por paciente
#define PRE_CLASSIFY_CLINICAL_RISK_TIME 1 // por paciente
#define PRE_CLASSIFY_SOCIAL_RISK_TIME 1

enum ManagerEvents {
    ASK_CONSENT,
    PRE_CLASSIFY_CLINICAL_RISK,
    PRE_CLASSIFY_SOCIAL_RISK,
    MANAGE_MEDICAL_HOUR,
    MANAGE_TEST_HOUR,
    RE_EVALUATE_LOW_RISK, 
    RE_EVALUATE_MANAGED
};

class Patient;

class Manager : public Agent {

private:
    static int _curr_id;
    int id;
    Cesfam * cesfam;
    map<Patient *, bool> patients; // false --> no intervenido; true --> intervenido

public:
    Manager(Cesfam * _cesfam, vector<Patient *> _patients, EventList * _event_list);

    int getId();

    void managePatient();

    void processEvent(Event * e) override;
};

#endif // !MANAGER_H