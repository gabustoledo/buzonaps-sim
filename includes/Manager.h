#ifndef MANAGER_H
#define MANAGER_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Patient.h"

#define ASK_CONSENT_TIME 1 // por paciente
#define PRE_CLASSIFY_CLINICAL_RISK_TIME 1 // por paciente
#define PRE_CLASSIFY_SOCIAL_RISK_TIME 1
#define MANAGE_PATIENT_TIME 1
#define RE_EVALUATE_LOW_RISK_TIME 1
#define RE_EVALUATE_MANAGED_TIME 1
#define MANAGE_MEDICAL_HOUR_TIME 1
#define MANAGE_TEST_HOUR_TIME 1
#define MANAGE_SOCIAL_HOUR_TIME 1
#define MANAGE_PSYCHO_HOUR_TIME 1

#define LOW_CLINICAL_RISK_PROB 0.1
#define MEDIUM_CLINICAL_RISK_PROB 0.5
#define HIGH_CLINICAL_RISK_PROB 0.4

#define LOW_SOCIAL_RISK_PROB 0.1
#define MEDIUM_SOCIAL_RISK_PROB 0.5
#define HIGH_SOCIAL_RISK_PROB 0.4

#define MEDICAL_HOUR_PROB 0.2
#define TEST_HOUR_PROB 0.2
#define SOCIAL_HOUR_PROB 0.2
#define PSYCHO_HOUR_PROB 0.2

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

public:
    /**
     * @brief Construct a new Manager object
     * 
     * @param _cesfam 
     * @param _patients 
     * @param _event_list 
     */
    Manager(Cesfam * _cesfam, vector<Patient *> _patients, EventList * _event_list);

    int getId();

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
};

#endif // !MANAGER_H