#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "Event.h"

class EventList {

private:
    Event * first = nullptr;
    double clock = 0.0;
    Event * last = nullptr;

};

#endif // !EVENTLIST_H