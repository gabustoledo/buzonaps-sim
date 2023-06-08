#ifndef SYSTEM_H
#define SYSTEM_H

#include "Patient.h"
#include "Manager.h"
#include "Cesfam.h"

class System {

private:
    Patient * patients;
    Manager * managers;
    Cesfam * cesfams;
    
};

#endif // !SYSTEM_H