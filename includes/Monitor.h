/**
 * @file Monitor.h
 * @author Manuel Ignacio Manriquez (@ManachoM - manuel.manriquez.b@usach.cl)
 * @brief 
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "glob.h"

class Monitor
{
private:
    ofstream outputFile; /** Objeto de escritura de archivos para registrar la salida*/
    mutex m; /** Mutex para las secciones críticas medias loquillas que encontramos por ahí*/
    condition_variable hasElements; /** Variable de condición que permite notificar a las hebras que hay entradas por escribir.*/
    thread logger_thread; /** Hebra que loggea, qué más puedo decir?*/
    bool debugFlag; /** Flag para imprimir por consola los logs*/
    queue<string> cola; /** Cola con los logs a escribir*/

    /**
     * @brief Método para pasarle a la hebra, loop infinito que verifica si hay elementos en la cola y los escribe.
     * 
     */
    void writeString();
public:

    /**
     * @brief Construct a new Monitor object
     * 
     * @param output_dir Ruta relativa donde se guardará el archivo
     * @param _debugFlag 
     */
    Monitor(string output_dir, bool _debugFlag = false);

    /**
     * @brief Destructor. Cierra el ofstream y mata a la hebra
     * 
     * @param obj 
     */
    void writeLog(json obj);

    /**
     * @brief Destroy the Monitor object, mata la hebra y cierra el ofstream
     * 
     */
    ~Monitor();

};

#endif // !_MONITOR_H_
