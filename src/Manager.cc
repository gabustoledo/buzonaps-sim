#include "../includes/Manager.h"

int Manager::_curr_id(0);

Manager::Manager(Cesfam * _cesfam, vector<Patient *> _patients, EventList * _event_list) {
    this->event_list = _event_list;
    this->cesfam = _cesfam;
    size_t i;
    for (i = 0; i < _patients.size(); i++) {
        this->patients.insert({_patients[i], false});
    }
}

int Manager::getId() {
    return this->id;
}

void Manager::processEvent(Event * e) {

    printf("EN GESTOR \n");
    Event * ev;

    switch (e->getEventType()) {
    case ManagerEvents::ASK_CONSENT:
        ev = new Event(CallerType::AGENT, this->getId(), e->getStartTime() + e->getExecTime(), PRE_CLASSIFY_CLINICAL_RISK_TIME, 
                        ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK, this, e->getCallerPtr(), nullptr);
        this->event_list->insertEvent(ev);
        break;
    
    case ManagerEvents::PRE_CLASSIFY_CLINICAL_RISK:

        break;

    case ManagerEvents::PRE_CLASSIFY_SOCIAL_RISK:

        break;

    case ManagerEvents::MANAGE_MEDICAL_HOUR:

        break;

    case ManagerEvents::MANAGE_TEST_HOUR:

        break;
    
    case ManagerEvents::RE_EVALUATE_LOW_RISK:

        break;

    case ManagerEvents::RE_EVALUATE_MANAGED:

        break;

    default:
        break;
    }
}
