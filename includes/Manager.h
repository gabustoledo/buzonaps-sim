#ifndef MANAGER_H
#define MANAGER_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Patient.h"
#include "SimConfig.h"
#include "Stats.h"

enum ManagerEvents {
    ASK_CONSENT,
    PRE_CLASSIFY_CLINICAL_RISK,
    PRE_CLASSIFY_SOCIAL_RISK,
    MANAGE_PATIENT,
    MANAGE_MEDICAL_HOUR,
    MANAGE_TEST_HOUR,
    MANAGE_SOCIAL_HOUR, 
    MANAGE_PSYCHO_HOUR,
    RE_EVALUATE_LOW_RISK, 
    RE_EVALUATE_MANAGED
};

class Patient;

/**
 * @brief 
 * 
 */
class Manager : public Agent {

private:
    static int _curr_id;
    int id;
    Cesfam * cesfam;
    map<Patient *, bool> patients; // false --> no intervenido; true --> intervenido
    int location;

public:
    /**
     * @brief Construct a new Manager object
     * 
     * @param _cesfam 
     * @param _patients 
     * @param _event_list 
     */
    Manager(Cesfam * _cesfam, EventList * _event_list, System * _system, int _location);

    int getId();

    int getLocation();

    void addPatient(Patient *);

    map<Patient *, bool> getPatients();

    void processEvent(Event * e) override;

    void processAskConsent(Event * e);

    void processPreClassifyClinicalRisk(Event * e);

    void processPreClassifySocialRisk(Event * e);

    void processReEvaluateLowRisk(Event * e);

    void processReEvaluateManaged(Event * e);

    void processManagePatient(Event * e);

    void processMedicalHour(Event * e);

    void processTestHour(Event * e);

    void processSocialHour(Event * e);

    void processPsychoHour(Event * e);

    double outOfTime() override;
};

#endif // !MANAGER_H