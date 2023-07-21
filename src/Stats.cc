#include "../includes/Stats.h"

Stats::Stats() {}

Stats * Stats::instance(nullptr);

Stats * Stats::getInstance() {
    if (instance == nullptr) {
        instance = new Stats();
    }
    return instance;
}

void Stats::updateManagedPatientsAmount() {
    this->managed_patients_amount++;
}

void Stats::updateTotalInterventions() {
    this->total_interventions++;
}

void Stats::updateDecreasedRiskEvents() {
    this->decreased_risk_events++;
}

void Stats::updateIncreasedRiskEvents() {
    this->increased_risk_events++;
}

void Stats::updateTotalMedicalHourReceived() {
    this->total_medical_hours_received++;
}

void Stats::updateTotalTestHourReceived() {
    this->total_test_hours_received++;
}

void Stats::updateTotalSocialHourReceived() {
    this->total_social_hours_received++;
}

void Stats::updateTotalPsychoHourReceived() {
    this->total_psycho_hours_received++;
}

void Stats::updateTotalMedicalHourAttended() {
    this->total_medical_hours_attended++;
}

void Stats::updateTotalTestHourAttended() {
    this->total_test_hours_attended++;
}

void Stats::updateTotalSocialHourAttended() {
    this->total_test_hours_attended++;
}

void Stats::updateTotalPsychoHourAttended() {
    this->total_psycho_hours_attended++;
}

void Stats::writeOutput(string filepath) {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ostringstream time;
    time << put_time(&tm, "%d%m%Y_%H-%M-%S");
    string output_str = filepath + "_" + time.str() + ".dat";
    ofstream general_output_file;
    general_output_file.open(output_str);
    json output;
    output["managed_patients_amount"] = this->managed_patients_amount;
    output["total_interventions"] = this->total_interventions;
    output["decreased_risk_events"] = this->decreased_risk_events;
    output["increased_risk_events"] = this->increased_risk_events;
    output["total_medical_hours_received"] = this->total_medical_hours_received;
    output["total_test_hours_received"] = this->total_test_hours_received;
    output["total_social_hours_received"] = this->total_social_hours_received;
    output["total_psycho_hours_received"] = this->total_psycho_hours_received;
    output["total_medical_hours_attended"] = this->total_medical_hours_attended;
    output["total_test_hours_attended"] = this->total_test_hours_attended;
    output["total_social_hours_attended"] = this->total_social_hours_attended;
    output["total_psycho_hours_attended"] = this->total_psycho_hours_attended;

    general_output_file << output << "\n";

    general_output_file.close();
}
