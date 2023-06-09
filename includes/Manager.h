#ifndef MANAGER_H
#define MANAGER_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Patient.h"

enum ManagerEvents {
    ASK_CONSENT,
    PRE_CLASIFY_CLINICAL_RISK,
    PRE_CLASIFY_SOCIAL_RISK,
    MANAGE_MEDICAL_HOUR,
    MANAGE_TEST_HOUR,
    RE_EVALUATE_LOW_RISK, 
    RE_EVALUATE_MANAGED
};

class Manager : public Agent {

private:
    static int _curr_id;
    int id;
    Cesfam * cesfam;
    map<Patient *, bool> patients; // false --> no intervenido; true --> intervenido

public:
    Manager(Cesfam * _cesfam, vector<Patient *> _patients);

    int getId();

    void managePatient();
};

#endif // !MANAGER_H