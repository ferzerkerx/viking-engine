/**
* @file HTMLEventLogger.cpp
* @brief Implementacin de la clase HTMLEventLogger
* @date Tuesday, October 23, 2007 4:34:18 PM
*/
#include "HTMLEventLogger.h"


#ifdef LOGGING


/**
* @copydoc TextEventLogger::lUpdate(int hours, int minutes, int seconds, int hundrethSeconds)
* @brief lo escribe con formato de HTML
*/
void HTMLEventLogger::lUpdate(int hours, int minutes, int seconds, int hundrethSeconds){
    writeStartParagraph();
    writeStartFont("Arial Black", 3, 0x00, 0x00, 0x00);
    writeText(0, "Actualizacion = %d, Tiempo = %.2d:%.2d:%.2d.%.2d", m_updateCount, hours, minutes, seconds, hundrethSeconds);
    writeEndFont();
    writeEndParagraph();
    writeEndLine();    
}


/**
* @brief Escribe la salida de texto con las banderas especificadas
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:38:05 PM
* @param buffer El texto a escribir en el HTML
* @param flags Banderas especificadas, estas indican color, sangria y tipo de texto
*/
void HTMLEventLogger::lOutput(const char* buffer, unsigned int flags){
    // Calculate text color
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    if(flags & LOG_COLOR_DK_RED)
        r = unsigned char(127);
    else if(flags & LOG_COLOR_RED)
        r = unsigned char(255);
    if(flags & LOG_COLOR_DK_GREEN)
        g = unsigned char(127);
    else if(flags & LOG_COLOR_GREEN)
        g = unsigned char(255);
    if(flags & LOG_COLOR_DK_BLUE)
        b = unsigned char(127);
    else if(flags & LOG_COLOR_BLUE)
        b = unsigned char(255);

    int size = 2;
    if(flags & LOG_SIZE_SMALL)
        size--;
    if(flags & LOG_SIZE_LARGE)
        size++;

    // Write formatted HTML to the output buffer
    writeIndent();
    writeStartListItem();
    writeStartFont("Arial", size, r, g, b);
    writeText(flags, buffer);
    writeEndFont();
    writeEndListItem();
    writeEndLine();
}

/**
* @copydoc TextEventLogger::lStartCallStackLevel(const char* fn)
* @brief lo escribe con formato de HTML
*/
void HTMLEventLogger::lStartCallStackLevel(const char* str){
    writeIndent();
	if (m_callStack.size() > 1){
		writeStartListItem();
		writeStartFont("Courier", 4, 0x99, 0x99, 0x99);
		writeText(LOG_BOLD, str);
		writeEndFont();
		writeEndListItem();
		writeStartUnsortedList();
		writeEndLine();

	}
	else{
		writeStartUnsortedList();
		writeStartFont("Courier", 4, 0x99, 0x99, 0x99);
		writeText(LOG_BOLD, str);
		writeEndFont();		
		writeEndLine();
	}	
}

/**
* @brief cierra los tags de HTML abiertos
*/
void HTMLEventLogger::lEndStackCallLevel(){
    writeIndent();
    writeEndUnsortedList();
	writeEndLine();
	if (m_callStack.size() < 1){
		writeEndUnsortedList();
		writeEndLine();
	}
}



/**
* @brief Escribe un tag de HTML de inicio de parrafo
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeStartParagraph(){
    fwrite("<p>", 1, 3, m_file);
}

/**
* @brief Escribe un tag de HTML de fin de parrafo
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeEndParagraph(){
    fwrite("</p>", 1, 4, m_file);
}

/**
* @brief Escribe un tag de HTML de inicio de lista desordenada
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeStartUnsortedList(){
    fwrite("<ul>", 1, 4, m_file);
}

/**
* @brief Escribe un tag de HTML de fin de lista desordenana
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeEndUnsortedList(){
    fwrite("</ul>", 1, 5, m_file);
}

/**
* @brief Escribe un tag de HTML de inicio de elemento de lista
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeStartListItem(){
    fwrite("<li>", 1, 4, m_file);
}

/**
* @brief Escribe un tag de HTML de fin de elemento de lista
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeEndListItem(){
    fwrite("</li>", 1, 5, m_file);
}

/**
* @brief Escribe un tag de HTML de inicio de fuente
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeStartFont(const char* szFontName, int iSize, unsigned char red, unsigned char green, unsigned char blue){
    char buffer[MAX_DEBUG_LINE_LEN];
    int len = sprintf(buffer, "<font face=\"%s\" size=\"%d\" color=\"#%.2x%.2x%.2x\">", szFontName, iSize, red, green, blue);
    assert(len < MAX_DEBUG_LINE_LEN-1);
    fwrite(buffer, 1, strlen(buffer), m_file);
}

/**
* @brief Escribe un tag de HTML de fin de fuente
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:39:22 PM
*/
void HTMLEventLogger::writeEndFont(){
    fwrite("</font>", 1, 7, m_file);
}


/**
* @brief Escribe el texto en el HTML con el formato especificado por
* las banderas
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:41:44 PM
* @param flags Banderas de formato de texto 
* @param lpszFormat  formato del texto a escribir (formato de texto estilo printf)
* @param ... los parametros usados por la cadena de texto formateada
*/
void HTMLEventLogger::writeText(unsigned int flags, const char* lpszFormat, ...){
    char buffer[MAX_DEBUG_LINE_LEN];
    va_list args; 
    va_start(args, lpszFormat); 
    int nBuf = _vsnprintf(buffer, MAX_DEBUG_LINE_LEN, lpszFormat, args); 
    assert((nBuf >= 0) && (nBuf < MAX_DEBUG_LINE_LEN)); 
    va_end(args);

   //escribir modificadores de texto
    if(flags & LOG_BOLD)
        fwrite("<b>", 1, 3, m_file);
    if(flags & LOG_ITALICS)
        fwrite("<i>", 1, 3, m_file);
    if(flags & LOG_UNDERLINE)
        fwrite("<u>", 1, 3, m_file);

    // escribimos el texto
    fwrite(buffer, 1, strlen(buffer), m_file);

    // cerramos los modificadores de texto
    if(flags & LOG_BOLD)
        fwrite("</b>", 1, 4, m_file);
    if(flags & LOG_ITALICS)
        fwrite("</i>", 1, 4, m_file);
    if(flags & LOG_UNDERLINE)
        fwrite("</u>", 1, 4, m_file);
}


/**
* @brief Escribe un salto de linea
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:45:03 PM
*/
void HTMLEventLogger::writeEndLine(){
    fwrite("\n", 1, 1, m_file);
}

/**
* @brief escribe un espacio de indentacion por cada nivel del callStack
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:45:22 PM
*/
void HTMLEventLogger::writeIndent(){
	for(unsigned int i = 0; i < m_previousStackLevel; i++){
        fwrite("  ", 1, 2, m_file);		
	}
}



/**
* @copydoc TextEventLogger::term()
*/
void HTMLEventLogger::term(){
	if (m_callStack.size() > 0){
		writeEndUnsortedList();
	}
	TextEventLogger::term();
}

#endif //logging