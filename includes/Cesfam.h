#ifndef CESFAM_H
#define CESFAM_H

#include "glob.h"

class Cesfam {

private:
    static int _curr_id;
    int id;
    int tests_hours_availability;
    int busy_test_hours;
    int medical_hours_availability;
    int busy_medical_hours;
    int location;

public:
    Cesfam();

    int getId();

};


#endif // !CESFAM_H