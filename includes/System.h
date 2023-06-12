#ifndef SYSTEM_H
#define SYSTEM_H

#include "glob.h"
#include "Patient.h"
#include "Manager.h"
#include "Cesfam.h"
#include "EventList.h"

class System {

private:
    EventList * event_list;
    map<int, Patient *> patients;
    vector<Patient *> patients_vector;
    map<int, Manager *> managers;
    map<int, Cesfam *> cesfams;

    void initializeCesfams();

    void initializePatients();

    void initializeManagers();
    
public:
    System(EventList * _event_list);

    void initializeSystem();

    void processEvent(Event * e);

    ~System();

};

#endif // !SYSTEM_H