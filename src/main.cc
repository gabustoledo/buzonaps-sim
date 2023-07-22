#include "../includes/main.h"

int main(int argc, char *argv[]) {

    int opt;
    string config_filepath = "";
    string out_filename = "";
    SimConfig * sim_config = nullptr;
    while((opt = getopt(argc, argv, "f:o:")) != -1) {
        switch(opt) {
            case 'f':
                config_filepath = optarg;
                sim_config = SimConfig::getInstance(config_filepath);
                break;
            case 'o':
                out_filename = optarg;
                break;
            case 'h':
                printf("HELP\n");
                break;
            default :
                printf("DEMO\n");
                break;
        }
    }

    if (config_filepath == "") { 
        config_filepath = "./config/config.json";
        sim_config = SimConfig::getInstance(config_filepath);
    }

    Stats * stats = Stats::getInstance();

    printf("INICIANDO SIMULACION\n");
    double END_SIM = sim_config->getParams()["end_sim"].get<double>();
    int cesfam_amount = sim_config->getParams()["cesfam_amount"].get<int>();
    int managers_amount = sim_config->getParams()["managers_amount"].get<int>();
    int patients_amount = sim_config->getParams()["patients_amount"].get<int>();

    EventList * event_list = new EventList(nullptr, 0.0, nullptr);

    System * sys = new System(event_list);
    sys->initializeSystem(cesfam_amount, managers_amount, patients_amount, out_filename);

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

    stats->writeOutput("./out/general");

    printf("TOTAL EVENTS = %d \n", event_list->getLastEvent()->getId());
    delete event_list;
    delete sys;

    exit(EXIT_SUCCESS);
}
