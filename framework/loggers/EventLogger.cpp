/**
* @file EventLogger.cpp
* @brief Implementacin de la clase EventLogger
* @date Tuesday, October 23, 2007 2:23:31 PM
*/
#include "EventLogger.h"
#include <string>
#include <windows.h>

#ifdef LOGGING


/**
* @brief Hace un push sobre nuestro EventLogger g_Log
* de la funcin especificada
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:36:54 PM
* @param funcion El nombre de la funcion en la que se
* esta haciendo el logging
*/
EventLogFN::EventLogFN(const char* funcion){
    g_Log->pushFunction(funcion);
}

/**
* @brief Hace un pop sobre nuestro EventLogger g_Log
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:37:00 PM
*/
EventLogFN::~EventLogFN(){
    g_Log->popFunction();
}

/**
* @brief Inicializa el stack y coloca al logger en su estado inicial
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:40:49 PM
*/
EventLogger::EventLogger(){
	m_updateCount = 0;
    m_b_loggedEvent = false;
    m_b_initialized = false;
    m_previousStackLevel = 0;
    m_callStack.reserve(32);
}


/**
* @brief coloca al objeto en un estado listo para 
* loggear eventos
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:41:43 PM
* @retval bool  Falso si ya ha sido inicializado, 
* verdadero si la inicializacion fue un exito
* @param logName Se debe especificar un nombre para el logger
* aun cuando no se necesite como en el ConsoleEventLogger, en caso
* de que el logger sea basado en archivo este nombre se usara para la
* creacion del archivo
*/
bool EventLogger::init(const char* logName){
    if(isInitialized())
        return false;

    m_b_initialized = true;
    m_b_loggedEvent = true;
    m_previousStackLevel = 0;
	
	return true;
}

/**
* @brief Nos dice si el objeto esta listo para loggear eventos
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:45:26 PM
* @retval bool  verdadero cuando est listo, falso cuando no lo esta
*/
bool EventLogger::isInitialized() { 
	return m_b_initialized; 
}

/**
* @brief Coloca al logger en un estado en que no puede loggear eventos
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:46:25 PM
*/
void EventLogger::term(){
    m_b_initialized = false;
}


/**
* @brief Loggea un evento con las banderas y formato especificados
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:47:16 PM
* @param format Es una cadena de texto en cierto formato, al estilo printf ("%s %d %c")
* @param ... Argumentos variables, usados por la cadena de formato
* @param flags Basicamente son defines, depende de cada tipo de logger hacer caso 
* o no a las banderas especificadas
*/
void EventLogger::logEvent(unsigned int flags, const char* format, ...){ 
    if(!isInitialized())
        return;
    char buffer[MAX_DEBUG_LINE_LEN];
    va_list args; 
    va_start(args, format); 
    int buf = _vsnprintf(buffer, MAX_DEBUG_LINE_LEN, format, args); 
    assert(buf >= 0); 
    va_end(args);
	
	/*se manda a loggear el evento con las banderas y
	la cadena de texto parseada*/
    logOutput(buffer, flags);
}

/**
* @brief Loggea un evento con las banderas y formato especificados
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:47:16 PM
* @param format Es una cadena de texto en cierto formato, al estilo printf ("%s %d %c")
* @param ... Argumentos variables, usados por la cadena de formato
*/
void EventLogger::logEvent(const char* format, ...){
    if(!isInitialized())
        return;
    char buffer[MAX_DEBUG_LINE_LEN];
    va_list args; 
    va_start(args, format); 

    int buf = _vsnprintf(buffer, MAX_DEBUG_LINE_LEN, format, args); 
    assert((buf >= 0) && (buf < MAX_DEBUG_LINE_LEN)); 
    va_end(args);
	
	/*se manda a loggear el evento con la cadena de texto parseada
	* y un cero que representa que no hay banderas
	*/
    logOutput(buffer, 0);
}


/**
* @brief loggea el callstack, loggea un evento si 
* es que hay uno y hace un flush
* esta funcion debe llamarse en el Game Loop
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:53:08 PM
*/
void EventLogger::update(){
    if(!isInitialized())
        return;

    m_updateCount++;

    
    logCallStack();
	
	if(m_b_loggedEvent){
        
        int hundrethSeconds = m_updateCount * 100 / 60 % 100;
        int seconds = m_updateCount / 60 % 60;
        int minutes = m_updateCount / 3600 % 60;
        int hours = m_updateCount / 216000;
		
		/**
		*invocamos a la funcion virtual del tipo de la instancia
		**/
		lUpdate(hours, minutes, seconds, hundrethSeconds);
   }
	
    m_b_loggedEvent = false;

    flush();

}


/**
* @brief Hace un push en nuestro callstack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:56:29 PM
* @param name Nombre de la funcion
*/
void EventLogger::pushFunction(const char* name){
    if(!isInitialized())
        return;
    m_callStack.push_back(name); 
}


/**
* @brief hace un pop en nuestro callStack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 2:56:57 PM
*/
void EventLogger::popFunction(){
    if(!isInitialized() || m_callStack.empty())
        return;
    m_callStack.pop_back();
    if(m_previousStackLevel >= m_callStack.size())
        logCallStack();
}


/**
* @brief Imprimimos el texto deseado con las banderas especificadas
* y se actualiza el callStack()
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 3:58:08 PM
* @param buffer El string parseado de impresion
* @param flags Las banderas de impresion
*/
void EventLogger::logOutput(char* buffer, unsigned int flags){
    // quitamos los caracteres de salto de linea '\n'
    int i = strlen(buffer);
    if(i == 0)
        return;
    if(buffer[i - 1] == '\n')
        buffer[i - 1] = 0;

    // indicamos que hay algo que imprimir
    m_b_loggedEvent = true;

    //actualizamos el callStack
    logCallStack();
	
	/**invocamos a la funcion virtual**/
	lOutput(buffer,flags);
}


/**
* @brief Loggea y actualiza el callStack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:01:58 PM
*/
void EventLogger::logCallStack(){
    unsigned int currentStackLevel = m_callStack.size();

    while(m_previousStackLevel < currentStackLevel)
    {	
		/**invocamos a la funcion virtual**/
		lStartCallStackLevel(m_callStack[m_previousStackLevel]);
        m_previousStackLevel++;
    }

    while(m_previousStackLevel > currentStackLevel)
    {
		/**invocamos a la funcion virtual**/
		lEndStackCallLevel();
        m_previousStackLevel--;
    }
    
}

#endif // LOGGING
