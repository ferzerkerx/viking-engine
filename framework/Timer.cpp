/**
* @file Timer.cpp
* @brief Implementacin de la clase Timer
* @date Saturday, August 11, 2007 10:14:52 PM
*/

#include "Timer.h"

/**
 * @brief Constructor de la clase Timer
 * inicializa las 2 variables, el que se resetea manual
 * y tambien el automtico
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:15:01 PM
 */
Timer::Timer(){
	ahora = GetTickCount();
	despues = GetTickCount();
	ultima = GetTickCount();
}

/**
 * @brief Destructor de la clase Timer
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:16:03 PM
 */
Timer::~Timer(){

}

/**
 * @brief Regresa la cantidad de milisegundos transcurridos desde
 * el ltimo reset()
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:16:26 PM
 * @retval Milisegundos transcurridos desde el reset()
 */
int Timer::getMilliseconds(){
	despues = GetTickCount();
	return despues-ahora;
}

/**
 * @brief Regresa la cantidad de milisegundos transcurridos desde
 * la ltima llamada a esta funcin
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:17:07 PM
 * @retval Milisegundos transcurridos desde la ltima llamada a esta funcin
 */
int Timer::getMillisecondsFromLastCall(){
	int res = ultima-GetTickCount(); 
	ultima = GetTickCount();
	return res; 

}

/**
 * @brief Regresa los segundos transcurridos desde
 * el ltimo reset()
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:18:21 PM
 * @retval Milisegundos transcurridos desde el ltimo reset()
 */
int Timer::getSeconds(){
	despues = GetTickCount();
	return (despues-ahora)/1000;

}

/**
 * @brief Regresa los segundos transcurridos desde
 * la ltima llamada a esta funcin
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:18:21 PM
 * @retval Milisegundos transcurridos desde la ltima llamada a esta funcin
 */
int Timer::getSecondsFromLastCall(){
	int res = ultima-GetTickCount(); 
	ultima = GetTickCount();
	return res/1000;
}

/**
 * @brief Resetea el timer
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:19:43 PM
 */
void Timer::reset(){
	ahora = despues;
}

