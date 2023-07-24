/**
 * @file Manager.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MANAGER_H
#define MANAGER_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Patient.h"
#include "SimConfig.h"
#include "Stats.h"

/**
 * @brief Enum que define los tipos de eventos asociados al tipo de agente gestor.
 * 
 */
enum ManagerEvents {
    ASK_CONSENT, /*!< Tipo de evento en que el gestor pregunta sobre el consentimiento informado a un paciente */
    PRE_CLASSIFY_CLINICAL_RISK, /*!< Tipo de evento en que el gestor pre clasifica el riesgo clínico de un paciente */
    PRE_CLASSIFY_SOCIAL_RISK, /*!< Tipo de evento en que el gestor pre clasifica el riesgo social de un paciente */
    MANAGE_PATIENT, /*!< Tipo de evento que con el que el gestor inicia la gestión de un paciente */
    MANAGE_MEDICAL_HOUR, /*!< Tipo de evento en que el gestor gestiona una hora médica */
    MANAGE_TEST_HOUR, /*!< Tipo de evento en que el gestor gestiona una hora a exámenes */
    MANAGE_SOCIAL_HOUR, /*!< Tipo de evento en que el gestor gestiona una hora para servicio social */
    MANAGE_PSYCHO_HOUR, /*!< Tipo de evento en que el gestor gestiona una hora de atención psicológica */
    RE_EVALUATE_LOW_RISK, /*!< Tipo de evento en que el gestor inicia la reevaluación de un paciente que obtuvo riesgo bajo */
    RE_EVALUATE_MANAGED /*!< Tipo de evento en que el gestor inicia la reevaluación de un paciente que obtuvo riesgo alto o moderado y que, por lo tanto, fue gestionado */
};

class Patient;

/**
 * @brief Clase que representa al agente de tipo gestor de casos.
 * 
 */
class Manager : public Agent {

private:
    static int _curr_id; /** Variable estática que lleva la cuenta de los identificadores asociados a la clase */
    int id; /** Identificador único del objeto Manager en la simulación */
    Cesfam * cesfam; /** Puntero al CESFAM al que pertenece el gestor */
    map<Patient *, bool> patients; /** Mapa de pacientes, booleano que indica si el paciente ha sido intervenido o no. false --> no intervenido; true --> intervenido */
    int location; /** Número que representa la ubicación del gestor */

public:
    /**
     * @brief Construye un nuevo objeto Manager
     * 
     * @param _cesfam Puntero al Cesfam al que pertenece el gestor
     * @param _event_list Puntero a la lista de eventos de la simulación
     * @param _system Puntero al sistema de la simulación
     * @param _location Número que representa la ubicación del gestor
     */
    Manager(Cesfam * _cesfam, EventList * _event_list, System * _system, int _location);

    /**
     * @brief Obtiene el identificador del gestor en la simulación
     * 
     * @return int 
     */
    int getId();

    /**
     * @brief Obtiene la ubicación del gestor
     * 
     * @return int 
     */
    int getLocation();

    /**
     * @brief Agrega un paciente al mapa de pacientes
     * 
     */
    void addPatient(Patient *);

    /**
     * @brief Obtiene el mapa de pacientes
     * 
     * @return map<Patient *, bool> 
     */
    map<Patient *, bool> getPatients();

    /**
     * @brief Genera el procesamiento completo de un evento (contiene a los demás métodos que procesan eventos)
     * 
     * @param e El puntero al evento a procesar
     */
    void processEvent(Event * e) override;
    
    /**
     * @brief Procesa un evento tipo ASK_CONSENT
     * 
     * @param e El puntero al evento a procesar
     */
    void processAskConsent(Event * e);

    /**
     * @brief Procesa un evento tipo PRE_CLASSIFY_CLINICAL_RISK
     * 
     * @param e El puntero al evento a procesar
     */
    void processPreClassifyClinicalRisk(Event * e);

    /**
     * @brief Procesa un evento tipo PRE_CLASSIFY_SOCIAL_RISK
     * 
     * @param e El puntero al evento a procesar
     */
    void processPreClassifySocialRisk(Event * e);

    /**
     * @brief Procesa un evento tipo RE_EVALUATE_LOW_RISK
     * 
     * @param e El puntero al evento a procesar
     */
    void processReEvaluateLowRisk(Event * e);

    /**
     * @brief Procesa un evento tipo RE_EVALUATE_MANAGED
     * 
     * @param e El puntero al evento a procesar
     */
    void processReEvaluateManaged(Event * e);

    /**
     * @brief Procesa un evento tipo MANAGE_PATIENT
     * 
     * @param e El puntero al evento a procesar
     */
    void processManagePatient(Event * e);

    /**
     * @brief Procesa un evento tipo MANAGE_MEDICAL_HOUR
     * 
     * @param e El puntero al evento a procesar
     */
    void processMedicalHour(Event * e);

    /**
     * @brief Procesa un evento tipo MANAGE_TEST_HOUR
     * 
     * @param e El puntero al evento a procesar
     */
    void processTestHour(Event * e);

    /**
     * @brief Procesa un evento tipo MANAGE_SOCIAL_HOUR
     * 
     * @param e El puntero al evento a procesar
     */
    void processSocialHour(Event * e);

    /**
     * @brief Procesa un evento tipo MANAGE_PSYCHO_HOUR
     * 
     * @param e El puntero al evento a procesar
     */
    void processPsychoHour(Event * e);

    /**
     * @brief Método que indica la cantidad de horas que faltan para el inicio de la jornada de un agente, 
     * retorna 0 si el agente se encuentra dentro del horario de su jornada.
     * 
     * @return double 
     */
    double outOfTime() override;
};

#endif // !MANAGER_H