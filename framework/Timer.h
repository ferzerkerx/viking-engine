/**
* @file Timer.h
* @brief Encabezado de la clase Timer
* @date Saturday, August 11, 2007 9:52:54 PM
*/
#ifndef __TIMER__
#define __TIMER__

#include <windows.h>

class Timer {
	private:

        DWORD ahora;//tiempo registrado en el reset
		DWORD despues;//tiempo actual
		DWORD ultima;//usado en funciones Lastcall
	public:
		Timer();
		~Timer();
		int getMilliseconds();
		int getMillisecondsFromLastCall();
		int getSeconds();
		int getSecondsFromLastCall();
		void reset();

};

#endif

/**
* @var Timer::ahora
* @brief registrado en el reset
*/

/**
*@var Timer::despues
* @brief tiempo actual
*/

/**
*@var Timer::ultima
* @brief usado en funciones Lastcall
*/