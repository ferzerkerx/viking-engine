/**
* @file ConsoleEventLogger.cpp
* @brief Implementacin de la clase ConsoleEventLogger
* @date Tuesday, October 23, 2007 5:07:44 PM
*/

#include "ConsoleEventLogger.h"

#ifdef LOGGING

/**
* @brief Imprime el texto en consola ignorando las banderas
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:07:57 PM
* @param buffer Texto a escribir en consola
* @param flags Banderas para impresion
* @warning Esta funcion ignora las banderas
*/
void ConsoleEventLogger::lOutput(const char *buffer, unsigned int flags) {
    printf(" %s\n", buffer);
}

/**
* @brief Imprime la funcion invocada en consola
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:07:57 PM
* @param str nombre de la funcion
*/
void ConsoleEventLogger::lStartCallStackLevel(const char *str) {
    printf("%s\n", str);
}

/**
* @brief Imprime un salto de linea despues de la impresion del callStack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:12:12 PM
*/
void ConsoleEventLogger::lEndStackCallLevel() {
    printf("\n");
}

/**
* @brief Imprime el texto de actualizacion
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:28:51 PM
* @param hours Horas transcurridas
* @param minutes Minutos transcurridos
* @param seconds Segundos transcurridos
* @param hundrethSeconds Centesimas de segundos transcurridos
*/
void ConsoleEventLogger::lUpdate(int hours, int minutes, int seconds, int hundrethSeconds) {
    printf("Actualizacion = %d, Tiempo = %.2d:%.2d:%.2d.%.2d\n", m_updateCount, hours, minutes, seconds,
           hundrethSeconds);
}


/**
* @brief Como es consola el flush no tiene ningun efecto
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:12:12 PM
*/
void ConsoleEventLogger::flush() {
    //Nothing to do here
}

#endif //logging