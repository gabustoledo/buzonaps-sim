#include "../includes/Cesfam.h"

int Cesfam::_curr_id(0);

Cesfam::Cesfam() : id(++_curr_id) {
    this->tests_hours_availability = 10;
    this->busy_test_hours = 0;
    this->medical_hours_availability = 10;
    this->busy_medical_hours = 0;
}

int Cesfam::getId(){
    return this->id;
}
