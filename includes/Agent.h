/**
 * @file Agent.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef AGENT_H
#define AGENT_H

#include "Event.h"
#include "EventList.h"

class EventList;
class Event;
class System;

/**
 * @brief Clase que representa a un agente en la simulación.
 * 
 */
class Agent {

protected: 
    EventList * event_list; /** Puntero a la lista de eventos de la simulación */
    System * system; /** Puntero al sistema de la simulación */

public:
    /**
     * @brief Método que procesa un evento.
     * 
     * @param e El puntero al evento a procesar.
     */
    virtual void processEvent(Event * e);

    /**
     * @brief Método que indica la cantidad de horas que faltan para el inicio de la jornada de un agente, 
     * retorna 0 si el agente se encuentra dentro del horario de su jornada.
     * 
     * @return double 
     */
    virtual double outOfTime();

};

#endif // !AGENT_H