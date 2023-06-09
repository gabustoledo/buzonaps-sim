#ifndef AGENT_H
#define AGENT_H

#include "Event.h"
#include "EventList.h"

class EventList;
class Event;

class Agent {

private: 
    EventList * event_list;

public:
    void processEvent(Event * e);

};

#endif // !AGENT_H