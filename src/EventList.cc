#include "../includes/EventList.h"

EventList::EventList(Event * _first, double _clock, Event * _last) {
    this->first = _first;
    this->clock = _clock;
    this->last = _last;
}

void EventList::insertEvent(Event * e) {
    Event *current;
    Event *last;

    if (this->first == nullptr) {
        this->first = e;
        this->last = e;
        return;
    }

    if(this->last != nullptr && (this->last->getStartTime() + this->last->getExecTime()) <= (e->getStartTime() + e->getExecTime())) {
        this->last->setNextEvent(e);
        this->last = e;
        return;
    }

    for (current = this->first, last = nullptr; current != nullptr; last = current, current = current->getNextEvent()) {
        if (e->getExecTime() + e->getStartTime() > current->getExecTime() + current->getStartTime())
            continue;
        e->setNextEvent(current);
        if (last != nullptr)
            last->setNextEvent(e);
        else
            this->first = e;
        return;
    }
    last->setNextEvent(e);
    this->last = e;
}

Event * EventList::nextEvent() {
    Event *e = this->first;
    this->first = this->first->getNextEvent();
    this->clock = e->getStartTime() + e->getExecTime();
    return e;
}

double EventList::getClock() {
    return this->clock;
}

bool EventList::isEmpty() {
    return this->first == nullptr;
}

EventList::~EventList() {}
