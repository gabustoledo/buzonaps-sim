#include "../includes/EventList.h"

EventList::EventList(Event * _first, double _clock, Event * _last) {
    this->first = _first;
    this->clock = _clock;
    this->last = _last;
}

void EventList::insertEvent(Event * e) {
    if (this->first == nullptr) {
        this->first = e;
        this->last = e;

        return;
    }
}
