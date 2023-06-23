#include "../includes/main.h"

int main(int argc, char *argv[]) {
    printf("INICIANDO SIMULACION\n");
    double END_SIM = 1000.0;
    int patients_amount = 10;

    EventList * event_list = new EventList(nullptr, 0.0, nullptr);

    System * sys = new System(event_list);
    sys->initializeSystem(patients_amount);

    Agent * agent;

    Event * e = new Event();

    while(!event_list->isEmpty() && event_list->getClock() <= END_SIM) {
        double last_clock = event_list->getClock();
        e = event_list->nextEvent();
        assert(e != nullptr);

        if(e->getCallerType() != CallerType::SYSTEM && e->getCallerPtr() != nullptr)
        {
            agent = e->getObjectivePtr();
            assert(last_clock <= event_list->getClock());
            printf("----------------------------------------------------------------------------------------------------\n");
            printf("[ID EVENT %d] - [CLOCK %lf]\n", e->getId(), event_list->getClock());
            agent->processEvent(e);
            continue; 
        }
        sys->processEvent(e);
    }
    delete event_list;
    delete sys;
    if(e != nullptr)
        delete e;

    exit(EXIT_SUCCESS);
}