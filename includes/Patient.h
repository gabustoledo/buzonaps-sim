/**
 * @file Patient.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PATIENT_H
#define PATIENT_H

#include "glob.h"
#include "Agent.h"
#include "Cesfam.h"
#include "Manager.h"
#include "System.h"
#include "SimConfig.h"
#include "Stats.h"

/**
 * @brief Enum que define las categorías de riesgos y sus valores
 * 
 */
enum RiskCategories {
    HIGH = 30, /*!< Riesgo alto */
    MEDIUM = 20, /*!< Riesgo moderado */
    LOW = 10, /*!< Riesgo bajo */
    UNDEFINED = 0 /*!< No definido */
};

/**
 * @brief Enum que define los tipos de eventos asociados al tipo de agente paciente.
 * 
 */
enum PatientEvents {
    ANSWER_CONSENT, /*!< Tipo de evento en que el paciente responde sobre el consentimiento informado */
    RECEIVE_MEDICAL_HOUR, /*!< Tipo de evento en que el paciente es informado sobre una hora médica agendada */
    RECEIVE_TEST_HOUR, /*!< Tipo de evento en que el paciente es informado sobre una hora a exámenes agendada */
    RECEIVE_SOCIAL_HOUR, /*!< Tipo de evento en que el paciente es informado sobre una hora para servicio social agendada */
    RECEIVE_PSYCHO_HOUR, /*!< Tipo de evento en que el paciente es informado sobre una hora de atención psicológica agendada */
    ATTEND_MEDICAL_HOUR, /*!< Tipo de evento en que el paciente asiste a una hora médica agendada */
    ATTEND_TEST_HOUR, /*!< Tipo de evento en que el paciente asiste a una hora a exámenes agendada */
    ATTEND_SOCIAL_HOUR, /*!< Tipo de evento en que el paciente asiste a una hora para servicio social agendada */
    ATTEND_PSYCHO_HOUR /*!< Tipo de evento en que el paciente asiste a una hora de atención psicológica agendada */
};

// ANSWER_CONSENT
// RECEIVE_MEDICAL_HOUR
// RECEIVE_TEST_HOUR
// RECEIVE_SOCIAL_HOUR
// RECEIVE_PSYCHO_HOUR
// ATTEND_MEDICAL_HOUR
// ATTEND_TEST_HOUR
// ATTEND_SOCIAL_HOUR
// ATTEND_PSYCHO_HOUR

/**
 * @brief Enum que define los tipos de horas que pueden ser agendadas por los gestores para los pacientes.
 * 
 */
enum HoursTypes {
    MEDICAL_HOUR, /*!< Hora médica */
    TEST_HOUR, /*!< Hora a exámenes */
    SOCIAL_HOUR, /*!< Hora para servicio social */
    PSYCHO_HOUR /*!< Hora de atención psicológica */
};



class Manager;

/**
 * @brief Clase que representa al agente de tipo paciente en la simulación.
 * 
 */
class Patient : public Agent {

private:
    static int _curr_id; /** Variable estática que lleva la cuenta de los identificadores asociados a la clase */
    int id; /** Identificador único del objeto Patient en la simulación */
    RiskCategories clinical_risk = RiskCategories::UNDEFINED; /** Riesgo clínico del paciente */
    RiskCategories social_risk = RiskCategories::UNDEFINED; /** Riesgo social del paciente */
    Cesfam * cesfam; /** Puntero al Cesfam al que pertenece el paciente */
    Manager * manager; /** Puntero al gestor (Manager) al que está asociado el paciente */
    map<HoursTypes, int> hours_attended; /** Mapa con los tipos de horas y cantidad de veces que ha asistido el paciente (si no asiste a una hora agendada, se le resta) */
    bool managed_state = false; /** Indicador de si el paciente ya ha sido gestionado una vez */
    int location; /** Número que representa la ubicación del paciente */

    /**
     * @brief Clasifica un número entero como categoría de riesgo
     * 
     * @param value El número entero a clasificar
     * @return RiskCategories 
     */
    RiskCategories classifyIntRisk(int value);

    /**
     * @brief Modifica el mapa con la asistencia a horas del paciente
     * 
     * @param type Tipo de hora a la que se le desea modificar la asistencia
     * @param amount Cantidad que se desea modificar (actualmente siempre se llama con valor 1)
     */
    void setHoursAttended(HoursTypes type, int amount);

public:
    /**
     * @brief Construct a new Patient object
     * 
     * @param _event_list Puntero a la lista de eventos de la simulación
     * @param system Puntero al sistema de la simulación
     * @param _location Número que representa la ubicación del paciente
     */
    Patient(EventList * _event_list, System * system, int _location);

    /**
     * @brief Obtiene el identificador del gestor en la simulación
     * 
     * @return int 
     */
    int getId();

    /**
     * @brief Obtiene el puntero al gestor asociado al paciente
     * 
     * @return Manager* 
     */
    Manager * getManager();

    /**
     * @brief Modifica el gestor al que se asocia el paciente
     * 
     * @param _manager El puntero al nuevo gestor del paciente
     */
    void setManager(Manager * _manager);

    /**
     * @brief Modifica el estado de gestionado del paciente
     *  
     * @param value Estado de gestionado del paciente (actualmente solo se llama con true)
     */
    void setManagedState(bool value);

    /**
     * @brief Obtiene el estado de gestionado del paciente
     * 
     * @return true 
     * @return false 
     */
    bool getManagedState();

    /**
     * @brief Obtiene la ubicación del gestor
     * 
     * @return int 
     */
    int getLocation();

    /**
     * @brief Modifica el riesgo clínico del paciente
     * 
     * @param _clinical_risk El nuevo riesgo clínico del paciente
     */
    void setClinicalRisk(RiskCategories _clinical_risk);

    /**
     * @brief Modifica el riesgo social del paciente
     * 
     * @param _clinical_social El nuevo riesgo social del paciente
     */
    void setSocialRisk(RiskCategories _clinical_social);

    /**
     * @brief Obtiene el riesgo clínico del paciente
     * 
     * @return RiskCategories 
     */
    RiskCategories getClinicalRisk();

    /**
     * @brief Obtiene el riesgo social del paciente
     * 
     * @return RiskCategories 
     */
    RiskCategories getSocialRisk();

    /**
     * @brief Obtiene el riesgo final del paciente (mayor entre ambos riesgos)
     * 
     * @return RiskCategories 
     */
    RiskCategories getFinalRisk();

    /**
     * @brief Calcula el riesgo final del paciente en base a su asistencia y/o inasistencia a horas gestionadas
     * 
     * @return RiskCategories 
     */
    RiskCategories calcFinalRisk();

    /**
     * @brief Genera el procesamiento completo de un evento (contiene a los demás métodos que procesan eventos)
     * 
     * @param e El puntero al evento a procesar
     */
    void processEvent(Event * e) override;

    /**
     * @brief Procesa un evento tipo ANSWER_CONSENT
     * 
     * @param e El evento a procesar
     */
    void processAnswerConsent(Event * e);

    /**
     * @brief Procesa un evento tipo RECEIVE_MEDICAL_HOUR
     * 
     * @param e El evento a procesar
     */
    void processReceiveMedicalHour(Event * e);

    /**
     * @brief Procesa un evento tipo RECEIVE_TEST_HOUR
     * 
     * @param e El evento a procesar
     */
    void processReceiveTestHour(Event * e);

    /**
     * @brief Procesa un evento tipo RECEIVE_SOCIAL_HOUR
     * 
     * @param e El evento a procesar
     */
    void processReceiveSocialHour(Event * e);

    /**
     * @brief Procesa un evento tipo RECEIVE_PSYCHO_HOUR
     * 
     * @param e El evento a procesar
     */
    void processReceivePsychoHour(Event * e);

    /**
     * @brief Procesa un evento tipo ATTEND_MEDICAL_HOUR
     * 
     * @param e El evento a procesar
     */
    void processAttendMedicalHour(Event * e);

    /**
     * @brief Procesa un evento tipo ATTEND_TEST_HOUR
     * 
     * @param e El evento a procesar
     */
    void processAttendTestHour(Event * e);

    /**
     * @brief Procesa un evento tipo ATTEND_SOCIAL_HOUR
     * 
     * @param e El evento a procesar
     */
    void processAttendSocialHour(Event * e);

    /**
     * @brief Procesa un evento tipo ATTEND_PSYCHO_HOUR
     * 
     * @param e El evento a procesar
     */
    void processAttendPsychoHour(Event * e);

};

#endif // !PATIENT_H