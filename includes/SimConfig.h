#ifndef SIM_CONFIG
#define SIM_CONFIG

#include "glob.h"

class SimConfig {

private:
    static SimConfig * instance;
    json params;

public: 
    SimConfig(string filepath);

    static SimConfig * getInstance(string filepath);

    json getParams();
};

#endif // !SIM_CONFIG