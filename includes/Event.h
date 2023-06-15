#ifndef EVENT_H
#define EVENT_H

#include "Agent.h"

enum CallerType {
    AGENT_MANAGER,
    AGENT_PATIENT, 
    SYSTEM
};

class Agent;

class Event {

private: 
    static int _curr_id;
    int id;
    short caller_type;
    int caller_id;
    double start_time;
    double exec_time;
    short event_type;
    Agent * caller_ptr;
    Agent * objective_ptr;
    Event * next_event;

public:
    Event(short _caller_type, int _caller_id, double _start_time, double _exec_time, short _event_type, Agent * _caller_ptr, Agent * _objective_ptr, Event * _next_event);

    Event();

    void setNextEvent(Event * e);

    double getStartTime();

    double getExecTime();

    Event * getNextEvent();

    Agent * getCallerPtr();

    Agent * getObjectivePtr();

    int getCallerId();

    int getId();

    short getCallerType();

    short getEventType();

};

#endif // !EVENT_H