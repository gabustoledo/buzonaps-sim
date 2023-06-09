#include "../includes/Event.h"

int Event::_curr_id(0);

Event::Event(short _caller_type, int _caller_id, double _start_time, double _exec_time, Agent * _caller_ptr, Event * _next_event) : id(++_curr_id) {
    this->caller_type = _caller_type;
    this->caller_id = _caller_id;
    this->start_time = _start_time;
    this->exec_time = _exec_time;
    this->caller_ptr = _caller_ptr;
    this->next_event = _next_event;
}

void Event::setNextEvent(Event * e) {
    this->next_event = e;
}
