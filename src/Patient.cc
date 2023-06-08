#include "../includes/Patient.h"

int Patient::_curr_id(0);

Patient::Patient(RiskCategories _clinical_risk, RiskCategories _social_risk) : id(++_curr_id) {
    this->clinical_risk = _clinical_risk;
    this->social_risk = _social_risk;
}

Patient::Patient(){}
