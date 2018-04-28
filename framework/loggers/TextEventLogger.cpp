/**
* @file TextEventLogger.cpp
* @brief Implementacin de la clase TextEventLogger
* @date Tuesday, October 23, 2007 4:21:30 PM
*/
#include "TextEventLogger.h"

#ifdef LOGGING

/**
* @brief Llama a EventLogger::init() e inicializa el archivo
* en el que se va a hacer el registro de eventos
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:22:00 PM
* @retval bool  verdadero si se inicializo con exito
*				falso en caso contrario
* @param logName El nombre del archivo en el que se va
* a hacer el registro de eventos
*/
bool TextEventLogger::init(const char* logName){
	EventLogger::init(logName);
	m_file = fopen(logName, "w");

	if(!m_file)		{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( logName, drive, dir, fname, ext );
		std::string filePath = fname;
		filePath += ext;
		m_file = fopen(filePath.c_str(), "w");

		if(!m_file){
			return false;
		}
	}
	return true;
}

/**
* @brief Cierra el archivo de texto e invoca a 
* TextEventLogger::term()
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:24:32 PM
*/
void TextEventLogger::term(){
	if(m_file){ fclose(m_file); }
    m_file = NULL;
	EventLogger::term();
}

/**
* @brief Escribe el texto con las banderas especificadas
* @warning esta clase ignora las banderas
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:25:07 PM
* @param buffer Texto a escribir en el archivo
* @param flags Banderas de impresion
*/
void TextEventLogger::lOutput(const char* buffer, unsigned int flags){
	fprintf(m_file," %s \n", buffer);
}


/**
* @brief Escribe en el archivo el nombre de la funcion del callStack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:26:21 PM
* @param str Nombre de la funcion del callStack
*/
void TextEventLogger::lStartCallStackLevel(const char * str){
	fprintf(m_file,"%s \n", str);
}


/**
* @brief Simplemente escribe un salto de linea cuando termina el callstack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:27:24 PM
*/
void TextEventLogger::lEndStackCallLevel(){
	fprintf(m_file,"\n");
}


 /**
 * @brief Escribe el texto de actualizacion
 * @author Fernando Montes de Oca Cspedes
 * @date Tuesday, October 23, 2007 4:28:51 PM
 * @param hours Horas transcurridas
 * @param minutes Minutos transcurridos
 * @param seconds Segundos transcurridos
 * @param hundrethSeconds Centesimas de segundos transcurridos
 */
void TextEventLogger::lUpdate(int hours, int minutes, int seconds, int hundrethSeconds){
	fprintf(m_file,"Actualizacion = %d, Tiempo = %.2d:%.2d:%.2d.%.2d \n", m_updateCount, hours, minutes, seconds, hundrethSeconds);
}


/**
* @brief Hace un flush sobre el archivo abierto
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:31:44 PM
*/
void TextEventLogger::flush(){
	 if(!isInitialized())
        return;
     fflush(m_file);

}

#endif //logging