/**
* @file ConsoleEventLogger.h
* @brief Encabezado de la clase ConsoleEventLogger
* @date Tuesday, October 23, 2007 12:36:18 PM
*/
#include "EventLogger.h"

#ifndef __CONSOLEEVENTLOGGER_H__
#define __CONSOLEEVENTLOGGER_H__

#ifdef LOGGING

/**
* @class ConsoleEventLogger
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 12:36:25 PM
* @brief Clase que loggea los eventos en consola
* 
*/ 
class ConsoleEventLogger : public EventLogger{

	private:
		void lOutput(const char* buffer, unsigned int flags);
		void lStartCallStackLevel(const char * str);
		void lEndStackCallLevel();
		void lUpdate(int hours, int minutes, int seconds, int hundrethSeconds);
		
	public:
		void flush();
};

#else

/**esto se usa en el caso de que LOGGING este desactivado***/
class ConsoleEventLogger : public EventLogger{};

#endif //logging

#endif //class