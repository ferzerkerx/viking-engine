/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Mayo 2006
 *
 * Header de la Clase que inicializa OpenGL
 *
 *
 ************************************************************************/

#ifndef __OGLINIT_H
#define __OGLINIT_H

#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN    

#pragma comment(linker,"/entry:WinMainCRTStartup")
#pragma comment(linker,"/subsystem:console")

// Cargamos librerias necesarias
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include <gl\gl.h>			// Header para OpenGL32
#include <gl\glu.h>			// Header para GLU32
#include <gl\glaux.h>		// Header para Glaux
#include <math.h>			// Header para pow, sqrt
#include <sstream>			// Header para salida de texto


// Creamos la clase OglInit
class OglInit {
	
	public:
		HDC	   hDC;			// Dispositivo de contexto (Private GDI Device Context)
		HGLRC  hRC;			// Contexto de rendereo (Permanent Rendering Context)
		HWND   hWnd;		// Manjeador de la Ventana 
		bool   fullscreen;
		
		OglInit(); // Constructor
		~OglInit(); // Destructor

		// Mtodos
		bool Init(int width, int height, int bits = 24);
		GLvoid KillGLWin();	
		GLvoid ResizeGLScene(GLsizei width, GLsizei height);
		int SetGL();
};

#endif