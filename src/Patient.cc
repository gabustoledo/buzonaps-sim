#include "../includes/Patient.h"

int Patient::_curr_id(0);

Patient::Patient(RiskCategories _clinical_risk, RiskCategories _social_risk) : id(++_curr_id) {
    this->clinical_risk = _clinical_risk;
    this->social_risk = _social_risk;
}

Patient::Patient(){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    double prob = dist(gen);
    printf("prob:  %f", prob);
    double prob_social = dist(gen);
    printf("prob_social: %f", prob_social);
    if (prob < 0.3) {
        this->clinical_risk = RiskCategories::LOW;
    } else if (prob >= 0.3 && prob < 0.6) {
        this->clinical_risk = RiskCategories::MEDIUM;
    } else {
        this->clinical_risk = RiskCategories::HIGH;
    }

    if (prob_social < 0.3) {
        this->social_risk = RiskCategories::LOW;
    } else if (prob >= 0.3 && prob < 0.6) {
        this->social_risk = RiskCategories::MEDIUM;
    } else {
        this->social_risk = RiskCategories::HIGH;
    }
}

int Patient::getId() {
    return this->id;
}
