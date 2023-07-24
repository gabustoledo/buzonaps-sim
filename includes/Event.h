/**
 * @file Event.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef EVENT_H
#define EVENT_H

#include "Agent.h"

/**
 * @brief Enum que define los tipos de entidades que pueden generar un evento
 * 
 */
enum CallerType {
    /**
     * @brief Entidad tipo agente gestor.
     * 
     */
    AGENT_MANAGER,
    /**
     * @brief Entidad tipo agente paciente.
     * 
     */
    AGENT_PATIENT, 
    /**
     * @brief Entidad tipo sistema.
     * 
     */
    SYSTEM
};

class Agent;

/**
 * @brief Clase que representa un evento en la simulación.
 * 
 */
class Event {

private: 
    static int _curr_id; /** Variable estática que lleva la cuenta de los identificadores asociados a la clase */
    int id; /** Identificador único del objeto Event en la simulación */
    short caller_type; /** Tipo de entidad que genera el evento */
    int caller_id; /** Identificador de la entidad que genera el evento (actualmente para agentes) */
    double start_time; /** Valor del reloj de la simulación cuando debe iniciar el evento */
    double exec_time; /** Cantidad de tiempo que demora en efectuarse el evento */
    short event_type; /** El tipo de evento */
    Agent * caller_ptr; /** Puntero al agente que genera el evento */
    Agent * objective_ptr; /** Puntero al agente que debe resolver el evento */
    Event * next_event; /** Puntero al siguiente evento */

public:
    /**
     * @brief Construye un nuevo objeto Event
     * 
     * @param _caller_type Tipo de entidad que genera el evento
     * @param _caller_id Identificador de la entidad que genera el evento (actualmente para agentes)
     * @param _start_time Valor del reloj de la simulación cuando debe iniciar el evento
     * @param _exec_time Cantidad de tiempo que demora en efectuarse el evento
     * @param _event_type El tipo de evento
     * @param _caller_ptr Puntero al agente que genera el evento
     * @param _objective_ptr Puntero al agente que debe resolver el evento
     * @param _next_event Puntero al siguiente evento
     */
    Event(short _caller_type, int _caller_id, double _start_time, double _exec_time, short _event_type, Agent * _caller_ptr, Agent * _objective_ptr, Event * _next_event);

    /**
     * @brief Construct a new Event object
     * 
     */
    Event();

    /**
     * @brief Modifica el puntero al siguiente evento
     * 
     * @param e Puntero al siguiente evento
     */
    void setNextEvent(Event * e);

    /**
     * @brief Obtiene el valor del reloj de la simulación en que comienza el evento
     * 
     * @return double 
     */
    double getStartTime();

    /**
     * @brief Obtiene la cantidad de tiempo que demora en efectuarse el evento
     * 
     * @return double 
     */
    double getExecTime();

    /**
     * @brief Obtiene el puntero al siguiente evento
     * 
     * @return Event* 
     */
    Event * getNextEvent();

    /**
     * @brief Obtiene el puntero al agente que generó el evento
     * 
     * @return Agent* 
     */
    Agent * getCallerPtr();

    /**
     * @brief Obtiene el puntero al agente que debe resolver el evento
     * 
     * @return Agent* 
     */
    Agent * getObjectivePtr();

    /**
     * @brief Obtiene el identificador del agente que generó el evento
     * 
     * @return int 
     */
    int getCallerId();

    /**
     * @brief Obtiene el identificador del evento
     * 
     * @return int 
     */
    int getId();

    /**
     * @brief Obtiene el tipo de entidad que generó el evento
     * 
     * @return short 
     */
    short getCallerType();

    /**
     * @brief Obtiene el tipo de evento 
     * 
     * @return short 
     */
    short getEventType();

};

#endif // !EVENT_H