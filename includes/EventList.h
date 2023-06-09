#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "Event.h"

class EventList {

private:
    Event * first = nullptr;
    double clock = 0.0;
    Event * last = nullptr;

public:
    EventList(Event * _first, double _clock, Event * _last);

    void insertEvent(Event * e);

    Event * nextEvent();

    bool isEmpty();

    ~EventList();

};

#endif // !EVENTLIST_H