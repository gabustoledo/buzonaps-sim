/**
 * @file System.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "glob.h"
#include "Patient.h"
#include "Manager.h"
#include "Cesfam.h"
#include "EventList.h"
#include "Monitor.h"

class Patient;
class Manager;

/**
 * @brief Clase que representa al sistema de la simulación.
 * 
 */
class System {

private:
    EventList * event_list; /** Puntero a la lista de eventos */
    map<int, Patient *> patients; /** Mapa de pacientes en la simulación (identificador del paciente, puntero del paciente) */
    vector<Patient *> patients_vector; /** Vector con los punteros a los pacientes */
    map<int, Manager *> managers; /** Mapa de gestores en la simulación (identificador del gestor, puntero del gestor) */
    map<int, Cesfam *> cesfams; /** Mapa de CESFAM en la simulación (identificador del CESFAM, puntero del CESFAM) */
    Monitor * monitor; /** Puntero al monitor que escribe las trazas detalladas del simulador */
    map<int, Cesfam *> locations_cesfam; /** Mapa con las ubicaciones de los CESFAM */
    map<int, vector<Patient *>> locations_patients; /** Mapa con las ubicaciones de los pacientes */
    map<int, vector<Manager *>> locations_managers; /** Mapa con las ubicaciones de los gestores */

    /**
     * @brief Inicializa los CESFAM de la simulación
     * 
     * @param amount Cantidad de CESFAM en la simulación
     */
    void initializeCesfams(int amount);

    /**
     * @brief Inicializa los pacientes de la simulación
     * 
     * @param amount Cantidad de pacientes en la simulación
     * @param cesfam_amount Cantidad de CESFAM en la simulación
     */
    void initializePatients(int amount, int cesfam_amount);

    /**
     * @brief Inicializa los gestores de la simulación
     * 
     * @param amount Cantidad de gestores por CESFAM en la simulación
     * @param cesfam_amount Cantidad de CESFAM en la simulación
     */
    void initializeManagers(int amount, int cesfam_amount);

    /**
     * @brief Agrega un CESFAM con su ubicación al mapa con las ubicaciones de los CESFAM
     * 
     * @param location Número que representa la ubicación
     * @param cesfam Puntero al Cesfam
     */
    void addLocaltionsCesfam(int location, Cesfam * cesfam);

    /**
     * @brief Agrega un paciente con su ubicación al mapa con las ubicaciones de los pacientes
     * 
     * @param location Número que representa la ubicación
     * @param cesfam Puntero al paciente
     */
    void addLocationsPatients(int location, Patient * patient);

    /**
     * @brief Agrega un gestor con su ubicación al mapa con las ubicaciones de los gestor
     * 
     * @param location Número que representa la ubicación
     * @param cesfam Puntero al gestor
     */
    void addLocationsManagers(int location, Manager * manager);
    
public:
    /**
     * @brief Construct a new System object
     * 
     * @param _event_list Puntero a la lista de eventos de la simulación
     */
    System(EventList * _event_list);

    /**
     * @brief Agrega un log (datos de un evento resuelto) al archivo de trazas detalladas de la simulación
     * 
     * @param obj El objeto json que describe a un evento resuelto
     */
    void log(json obj);

    /**
     * @brief Inicializa el sistema completo
     * 
     * @param cesfam_amount Cantidad de CESFAM en la simulacion
     * @param manager_amount Cantidad de gestores por CESFAM en la simulación
     * @param patients_amount Cantidad de pacientes en la simulación
     * @param out_filename Indentificador extra (aparte del timestamp) para nombre del archivo de salida con las trazas detalladas
     * @param time Timestamp de inicio de la simulación en string
     */
    void initializeSystem(int cesfam_amount, int manager_amount, int patients_amount, string out_filename, string time);

    /**
     * @brief Procesa un evento
     * 
     * @param e Puntero al evento a procesar
     */
    void processEvent(Event * e);

    Patient *get_patient_by_id(int id);

    Manager *get_manager_by_id(int id);

    /**
     * @brief Destroy the System object
     * 
     */
    ~System();

};

#endif // !SYSTEM_H