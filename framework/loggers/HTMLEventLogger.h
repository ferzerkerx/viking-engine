/**
* @file HTMLEventLogger.h
* @brief Encabezado de la clase HTMLEventLogger
* @date Tuesday, October 23, 2007 1:35:17 PM
*/

#include "TextEventLogger.h"

#ifndef __HTMLEVENTLOGGER_H__
#define __HTMLEVENTLOGGER_H__

#ifdef LOGGING

/***Banderas de impresion***/
#define LOG_COLOR_RED           0x00000001
#define LOG_COLOR_DK_RED        0x00000002
#define LOG_COLOR_GREEN         0x00000004
#define LOG_COLOR_DK_GREEN      0x00000008
#define LOG_COLOR_BLUE          0x00000010
#define LOG_COLOR_DK_BLUE       0x00000020
#define LOG_SIZE_LARGE          0x00000040
#define LOG_SIZE_SMALL          0x00000080
#define LOG_BOLD                0x00000100
#define LOG_ITALICS             0x00000200
#define LOG_UNDERLINE           0x00000400


/**
* @class HTMLEventLogger
* @author Fernando Montes de Oca Cspedes
* @date Tuesday, October 23, 2007 4:33:43 PM
* @brief Esta clase escribe el log de eventos en formato
* HTML
*/ 
class HTMLEventLogger : public TextEventLogger{
	protected:
		void lOutput(const char* buffer, unsigned int flags);
		void lStartCallStackLevel(const char * str);
		void lEndStackCallLevel();
		void lUpdate(int hours, int minutes, int seconds, int hundrethSeconds);
		void writeStartParagraph();
        void writeEndParagraph();
        void writeStartUnsortedList();
        void writeEndUnsortedList();
        void writeStartListItem();
        void writeEndListItem();
        void writeStartFont(const char* fontName, int size, unsigned char red, unsigned char green, unsigned char blue);
        void writeEndFont();
        void writeText(unsigned int flags, const char* format, ...);
        void writeEndLine();
        void writeIndent();
		void term();
};


#else

/**esto se usa en el caso de que LOGGING este desactivado***/
class HTMLEventLogger : public TextEventLogger{
};

#define LOG_COLOR_RED           0
#define LOG_COLOR_DK_RED        0
#define LOG_COLOR_GREEN         0
#define LOG_COLOR_DK_GREEN      0
#define LOG_COLOR_BLUE          0
#define LOG_COLOR_DK_BLUE       0
#define LOG_SIZE_LARGE          0
#define LOG_SIZE_SMALL          0
#define LOG_BOLD                0
#define LOG_ITALICS             0
#define LOG_UNDERLINE           0


#endif //logging

#endif //class