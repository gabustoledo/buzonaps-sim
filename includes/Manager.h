#ifndef MANAGER_H
#define MANAGER_H

#include "Agent.h"
#include "Cesfam.h"

class Manager : public Agent {

private:
    static int _curr_id;
    int id;
    Cesfam * cesfam;
};

#endif // !MANAGER_H