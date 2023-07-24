/**
 * @file Cesfam.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CESFAM_H
#define CESFAM_H

#include "glob.h"

/**
 * @brief Clase que representa a un CESFAM en la simulación.
 * 
 */
class Cesfam {

private:
    static int _curr_id; /** Variable estática que lleva la cuenta del identificador de la clase */
    int id; /** Identificador único del CESFAM en la simulación */
    int tests_hours_availability; /** Cantidad de horas para exámenes disponibles en el CESFAM (no están siendo consideradas actualmente) */
    int busy_test_hours; /** Cantidad de horas para exámenes ocupadas en el CESFAM (no están siendo consideradas actualmente) */
    int medical_hours_availability; /** Cantidad de horas médicas disponibles en el CESFAM (no están siendo consideradas actualmente) */
    int busy_medical_hours; /** Cantidad de horas médicas ocupadas en el CESFAM (no están siendo consideradas actualmente) */
    int location; /** Número que representa la ubicación del CESFAM */

public:
    /**
     * @brief Construye un nuevo objeto Cesfam.
     * 
     */
    Cesfam();

    /**
     * @brief Obtiene el identificador del CESFAM en la simulación.
     * 
     * @return int 
     */
    int getId();

};


#endif // !CESFAM_H