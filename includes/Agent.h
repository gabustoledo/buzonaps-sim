#ifndef AGENT_H
#define AGENT_H

#include "Event.h"
#include "EventList.h"

class EventList;
class Event;
class System;

class Agent {

protected: 
    EventList * event_list;
    System * system;

public:
    virtual void processEvent(Event * e);

};

#endif // !AGENT_H