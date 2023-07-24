/**
 * @file EventList.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "glob.h"
#include "Event.h"

class Event;

/**
 * @brief Clase que representa la lista de eventos de la simulación.
 * 
 */
class EventList {

private:
    Event * first = nullptr; /** Puntero al primer evento de la lista */
    double clock = 0.0; /** Valor actual del reloj de la simulación */
    Event * last = nullptr; /** Puntero al último evento de la lista */

public:
    /**
     * @brief Construye un nuevo objeto EventList
     * 
     * @param _first Puntero al primer evento de la lista
     * @param _clock Valor actual del reloj de la simulación
     * @param _last Puntero al último evento de la lista
     */
    EventList(Event * _first, double _clock, Event * _last);

    /**
     * @brief Inserta un evento en la lista de eventos
     * 
     * @param e Puntero al evento a insertar
     */
    void insertEvent(Event * e);

    /**
     * @brief Obtiene el puntero al siguiente evento a resolver de la lista de eventos
     * 
     * @return Event* 
     */
    Event * nextEvent();

    /**
     * @brief Indica si la lista de eventos está vacía o no
     * 
     * @return true 
     * @return false 
     */
    bool isEmpty();

    /**
     * @brief Obtiene el valor actual del reloj de la simulación
     * 
     * @return double 
     */
    double getClock();

    /**
     * @brief Obtiene el puntero al último evento de la lista de eventos
     * 
     * @return Event* 
     */
    Event * getLastEvent();

    /**
     * @brief Destruye el objeto EventList
     * 
     */
    ~EventList();

};

#endif // !EVENTLIST_H