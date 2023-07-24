/**
 * @file Stats.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef STATS_H
#define STATS_H

#include <time.h>
#include "glob.h"
#include "SimConfig.h"

/**
 * @brief Clase que contiene las variables globales de la simulación.
 * 
 */
class Stats {

private:
    static Stats * instance; /** Puntero a la instancia Stats */

    int managed_patients_amount = 0; /** Cantidad de pacientes gestionados */
    int total_interventions = 0; /** Cantidad de intervenciones totales */
    int decreased_risk_events = 0; /** Cantidad de veces que un paciente disminuyó su riesgo respecto al de la evaluación anterior */
    int increased_risk_events = 0; /** Cantidad de veces que un paciente aumentó su riesgo respecto al de la evaluación anterior */
    int total_medical_hours_received = 0; /** Cantidad de horas médicas agendadas para los pacientes */
    int total_test_hours_received = 0; /** Cantidad de horas a exámenes agendadas para los pacientes */
    int total_social_hours_received = 0; /** Cantidad de horas para servicio social agendadas para los pacientes */
    int total_psycho_hours_received = 0; /** Cantidad de horas de atención psicológica agendadas para los pacientes */
    int total_medical_hours_attended = 0; /** Cantidad de horas médicas a las que los pacientes asistieron */
    int total_test_hours_attended = 0; /** Cantidad de horas a exámenes a las que los pacientes asistieron */
    int total_social_hours_attended = 0; /** Cantidad de horas para servicio social a las que los pacientes asistieron */
    int total_psycho_hours_attended = 0; /** Cantidad de horas de atención psicológica a las que los pacientes asistieron */

public: 
    /**
     * @brief Construct a new Stats object
     * 
     */
    Stats();

    /**
     * @brief Obtiene el puntero a la instancia Stats si no es nula, sino la crea
     * 
     * @return Stats* 
     */
    static Stats * getInstance();

    /**
     * @brief Suma 1 al atributo managed_patients_amount
     * 
     */
    void updateManagedPatientsAmount();

    /**
     * @brief Suma 1 al atributo total_interventions
     * 
     */
    void updateTotalInterventions();

    /**
     * @brief Suma 1 al atributo decreased_risk_events
     * 
     */
    void updateDecreasedRiskEvents();

    /**
     * @brief Suma 1 al atributo increased_risk_events
     * 
     */
    void updateIncreasedRiskEvents();

    /**
     * @brief Suma 1 al atributo total_medical_hours_received 
     * 
     */
    void updateTotalMedicalHourReceived();

    /**
     * @brief Suma 1 al atributo total_test_hours_received 
     * 
     */
    void updateTotalTestHourReceived();

    /**
     * @brief Suma 1 al atributo total_social_hours_received 
     * 
     */
    void updateTotalSocialHourReceived();

    /**
     * @brief Suma 1 al atributo total_psycho_hours_received 
     * 
     */
    void updateTotalPsychoHourReceived();

    /**
     * @brief Suma 1 al atributo total_medical_hours_attended
     * 
     */
    void updateTotalMedicalHourAttended();

    /**
     * @brief Suma 1 al atributo total_test_hours_attended
     * 
     */
    void updateTotalTestHourAttended();

    /**
     * @brief Suma 1 al atributo total_social_hours_attended
     * 
     */
    void updateTotalSocialHourAttended();
    
    /**
     * @brief Suma 1 al atributo total_psycho_hours_attended
     * 
     */
    void updateTotalPsychoHourAttended();

    /**
     * @brief Escribe el valor resultante de las variables
     * 
     * @param filepath La ruta al archivo de salida
     * @param time El timestamp de inicio de la simulación que se asociará al nombre del archivo
     */
    void writeOutput(string filepath, string time);

};

#endif // !STATS_H