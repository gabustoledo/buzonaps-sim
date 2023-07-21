#ifndef STATS_H
#define STATS_H

#include <time.h>
#include "glob.h"

/**
 * @brief 
 * 
 */
class Stats {

private:
    static Stats * instance;

    int managed_patients_amount = 0;
    int total_interventions = 0;
    int decreased_risk_events = 0;
    int increased_risk_events = 0;
    int total_medical_hours_received = 0;
    int total_test_hours_received = 0;
    int total_social_hours_received = 0;
    int total_psycho_hours_received = 0;
    int total_medical_hours_attended = 0;
    int total_test_hours_attended = 0;
    int total_social_hours_attended = 0;
    int total_psycho_hours_attended = 0;

public: 
    Stats();

    static Stats * getInstance();

    void updateManagedPatientsAmount();

    void updateTotalInterventions();

    void updateDecreasedRiskEvents();

    void updateIncreasedRiskEvents();

    void updateTotalMedicalHourReceived();

    void updateTotalTestHourReceived();

    void updateTotalSocialHourReceived();

    void updateTotalPsychoHourReceived();

    void updateTotalMedicalHourAttended();

    void updateTotalTestHourAttended();

    void updateTotalSocialHourAttended();

    void updateTotalPsychoHourAttended();

    void writeOutput(string filepath);

};

#endif // !STATS_H