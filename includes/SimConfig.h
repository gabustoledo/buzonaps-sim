/**
 * @file SimConfig.h
 * @author Florencia Corvalán Lillo (@FloCorvalan - florencia.corvalan@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2023-07-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SIM_CONFIG
#define SIM_CONFIG

#include "glob.h"

/**
 * @brief Clase que contiene los parámetros de configuración de la simulación.
 * 
 */
class SimConfig {

private:
    static SimConfig * instance; /** Puntero a la instancia SimConfig */
    json params; /** json con los parámetros de configuración de la simulación */

public:
    /**
     * @brief Construct a new Sim Config object
     * 
     * @param filepath La ruta del archivo de configuración desde donde se cargarán los parámetros
     */
    SimConfig(string filepath);

    /**
     * @brief Obtiene la instancia SimConfig si no es nula, sino la crea
     * 
     * @param filepath La ruta del archivo de configuración desde donde se cargarán los parámetros
     * @return SimConfig* 
     */
    static SimConfig * getInstance(string filepath);

    static SimConfig * getInstanceNew(string filepath);

    /**
     * @brief Obtiene el json con los parámetros de configuración de la simulación
     * 
     * @return json 
     */
    json getParams();
};

#endif // !SIM_CONFIG