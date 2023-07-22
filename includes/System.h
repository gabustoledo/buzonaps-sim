#ifndef SYSTEM_H
#define SYSTEM_H

#include "glob.h"
#include "Patient.h"
#include "Manager.h"
#include "Cesfam.h"
#include "EventList.h"
#include "Monitor.h"

class Patient;
class Manager;

class System {

private:
    EventList * event_list;
    map<int, Patient *> patients;
    vector<Patient *> patients_vector;
    map<int, Manager *> managers;
    map<int, Cesfam *> cesfams;
    Monitor * monitor;
    map<int, Cesfam *> locations_cesfam;
    map<int, vector<Patient *>> locations_patients;
    map<int, vector<Manager *>> locations_managers;

    void initializeCesfams(int amount);

    void initializePatients(int amount, int cesfam_amount);

    void initializeManagers(int amount, int cesfam_amount);

    void addLocaltionsCesfam(int location, Cesfam * cesfam);

    void addLocationsPatients(int location, Patient * patient);

    void addLocationsManagers(int location, Manager * manager);
    
public:
    System(EventList * _event_list);

    void log(json obj);

    void initializeSystem(int cesfam_amount, int manager_amount, int patients_amount, string out_filename);

    void processEvent(Event * e);

    ~System();

};

#endif // !SYSTEM_H