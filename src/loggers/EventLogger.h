/**
* @file EventLogger.h
* @brief Encabezado de la clase EventLogger
* @date Tuesday, October 23, 2007 2:23:31 PM
*/
#ifndef __EVENTLOGGER_H__
#define __EVENTLOGGER_H__



/**
* LOGGING Usada para habilitar/deshabilitar
* el loggeo de eventos
*/
//#define LOGGING

#ifdef LOGGING

#include <vector>
#include <map>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

/**macros que sirven para el loggeo de eventos**/
#define FN(var_1)                   EventLogFN obj____unique_name(var_1)
#define LOG                         g_Log->logEvent
#define LOG_INIT                    g_Log->init
#define LOG_UPDATE                  g_Log->update
#define LOG_TERM                    g_Log->term
#define LOG_FLUSH                   g_Log->flush

/**
* @var MAX_DEBUG_LINE_LEN
* @brief usado para la creacion de buffers
*/ 
const int MAX_DEBUG_LINE_LEN = 1024; //usado para la creacion de buffers

/**
* @class EventLogFN
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:28:41 PM
* @brief Esta clase nos ayuda a mantener un callstack
* cada vez que se invoca el macro FN(var_1) se crea un objeto
* que solo muere cuando la funcion termina por lo que alguna llamada
* a otra funcion con invocacion al macro FN(var_1) crea otro objeto
* de esta manera podemos llevar un callstack
*/ 
class EventLogFN {
public:
	EventLogFN(const char* funcion);
	~EventLogFN();
private:
	const char* m_function; // nombre de la funcion
};

/**
* @var EventLogFN::m_function
* @brief  nombre de la funcion
*/



typedef std::vector<const char*> CharPtrVec; //tipo que usaremos para nuestro callStack


/**
* @class EventLogger
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:22:31 PM
* @brief Clase abstracta que sirve de interfaz para los logger de Eventos
* especficos
*/
class EventLogger {
	public:
		EventLogger();		
		virtual bool init(const char* logName);
		virtual void term();
		bool isInitialized();
		void logEvent(unsigned int flags, const char* format, ...);
		void logEvent(const char* format, ...);
		void update();
		virtual void flush()=0;
		void pushFunction(const char* name);
		void popFunction();

	protected:

		virtual void lOutput(const char* buffer, unsigned int flags) = 0;
		virtual void lStartCallStackLevel(const char * str) = 0;
		virtual void lEndStackCallLevel() = 0;
		virtual void lUpdate(int hours, int minutes, int seconds, int hundrethSeconds) = 0;
		void logOutput(char* buffer, unsigned int flags);
		void logCallStack();

		
		CharPtrVec      m_callStack;	//almacena los nombres de las funciones llamadas
		bool            m_b_loggedEvent;	//indica si acaba de ocurrir un evento
		unsigned int    m_previousStackLevel;	//nos dice el nivel anterior en la pila de llamadas
		unsigned int    m_updateCount;		//utilizado para llevar la cuenta del tiempo
		bool            m_b_initialized;		//indica si el logger ha sido inicializado
};

/**
* @var EventLogger::m_callStack
* @brief almacena los nombres de las funciones llamadas
*/

/**
* @var EventLogger::m_b_loggedEvent
* @brief indica si acaba de ocurrir un evento
*/

/**
* @var EventLogger::m_previousStackLevel
* @brief nos dice el nivel anterior en la pila de llamadas
*/

/**
* @var EventLogger::m_updateCount
* @brief utilizado para llevar la cuenta del tiempo
*/

/**
* @var EventLogger::m_b_initialized
* @brief indica si el logger ha sido inicializado
*/


extern EventLogger *g_Log; //utilizada para los macros del logger

#else

/**
*estos valores se definen para que el compilador no marque error
* cuando no est habilitado el loggeo
**/

class EventLogger{};

#define LOG         printf
#define FN(x)
#define LOG_INIT(x)
#define LOG_UPDATE()
#define LOG_TERM()
#define LOG_FLUSH()


#endif //#ifdef LOGGING

#endif //class