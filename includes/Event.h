#ifndef EVENT_H
#define EVENT_H

#include "Agent.h"

enum CallerType {
    AGENT, 
    SYSTEM
};

class Event {

private: 
    static int _curr_id;
    int id;
    short caller_type;
    int caller_id;
    double start_time;
    double exec_time;
    Agent * caller_ptr;
    Event * next_event;
};

#endif // !EVENT_H