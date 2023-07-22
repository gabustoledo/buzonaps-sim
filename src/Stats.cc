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

void Stats::writeOutput(string filepath, string time) {
    SimConfig * sim_config = SimConfig::getInstance("");
    int cesfam_amount = sim_config->getParams()["cesfam_amount"].get<int>();
    int managers_amount = sim_config->getParams()["managers_amount"].get<int>();
    int patients_amount = sim_config->getParams()["patients_amount"].get<int>();
    
    string output_str = filepath + "_" + time + ".dat";
    ofstream general_output_file;
    general_output_file.open(output_str);
    json output;
    output["cesfam_amount"] = cesfam_amount;
    output["managers_amount"] = managers_amount;
    output["patients_amount"] = patients_amount;
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
