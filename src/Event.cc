#include "../includes/Event.h"

int Event::_curr_id(0);

Event::Event(short _caller_type, int _caller_id, double _start_time, double _exec_time, short _event_type, Agent * _caller_ptr, Agent * _objective_ptr, Event * _next_event) : id(++_curr_id) {
    this->caller_type = _caller_type;
    this->caller_id = _caller_id;
    this->start_time = _start_time;
    this->exec_time = _exec_time;
    this->event_type = _event_type;
    this->caller_ptr = _caller_ptr;
    this->objective_ptr = _objective_ptr;
    this->next_event = _next_event;
}

Event::Event() {}

void Event::setNextEvent(Event * e) {
    this->next_event = e;
}

double Event::getStartTime() {
    return this->start_time;
}

double Event::getExecTime() {
    return this->exec_time;
}

Event * Event::getNextEvent() {
    return this->next_event;
}

Agent * Event::getCallerPtr() {
    return this->caller_ptr;
}

Agent * Event::getObjectivePtr() {
    return this->objective_ptr;
}

int Event::getCallerId() {
    return this->caller_id;
}

int Event::getId() {
    return this->id;
}

short Event::getCallerType() {
    return this->caller_type;
}

short Event::getEventType() {
    return this->event_type;
}
