#ifndef AGENT_H
#define AGENT_H

#include "Event.h"
#include "EventList.h"

class EventList;
class Event;

class Agent {

protected: 
    EventList * event_list;

public:
    virtual void processEvent(Event * e);

};

#endif // !AGENT_H