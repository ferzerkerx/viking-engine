/**
* @file TextEventLogger.h
* @brief Encabezado de la clase TextEventLogger
* @date Tuesday, October 23, 2007 12:57:22 PM
*/
#include "EventLogger.h"

#ifndef __TEXTEVENTLOGGER_H__
#define __TEXTEVENTLOGGER_H__

#ifdef LOGGING

/**
* @class TextEventLogger
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:20:31 PM
* @brief Loggea los eventos en un archivo de texto
* con el nombre especificado en init
*/ 
class TextEventLogger : public EventLogger{
	public:
		bool init(const char* logName);
		void flush();
		void term();

	private:
		void lOutput(const char* buffer, unsigned int flags);
		void lStartCallStackLevel(const char * str);
		void lEndStackCallLevel();
		void lUpdate(int hours, int minutes, int seconds, int hundrethSeconds);
		

	protected:
		FILE*   m_file;	//apuntador al archivo

};

/**
* @var TextEventLogger::m_file
* @brief apuntador al archivo
*/


#else

/**esto se usa en el caso de que LOGGING este desactivado***/
class TextEventLogger : public EventLogger{
};
#endif //logging

#endif //class