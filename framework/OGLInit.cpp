/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Mayo 2006
 *
 * Clase que inicializa OpenGL
 *
 *
 ************************************************************************/

#include "OGLInit.h"

// Creamos constructor, inicializamos variables de inicializacin de OGL
OglInit::OglInit() {
	hDC = NULL;
	hRC = NULL;
	hWnd = NULL;
	fullscreen = TRUE;
}

// Creamos destructor
OglInit::~OglInit(){}

// Creamos mtodo de inicializacin de OGL
bool OglInit::Init(int width, int height, int bits) {

	GLuint PixelFormat;			

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); // Tamao de este Pixel Format Descriptor
    pfd.nVersion   = 1;						// Nmero de Version
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW |	// Soporte para Ventana
					 PFD_SUPPORT_OPENGL |	// Soporte para OpenGL 
					 PFD_DOUBLEBUFFER;		// Soporte para Double Buffering
    pfd.iPixelType = PFD_TYPE_RGBA;			// Peticin de formato RGBA
    pfd.cColorBits = bits;					// Color Depth
    pfd.cDepthBits = 16;					// 16Bit Z-Buffer (Depth Buffer)

		if(!(hDC = GetDC(hWnd))) {					// Tenemos Device Context?
		KillGLWin();							
		MessageBox(NULL,"Can't Create A GL Device Context.",
			       "ERROR",MB_OK|MB_ICONEXCLAMATION);
    	return FALSE;							
	}

	if(!(PixelFormat = ChoosePixelFormat(hDC,&pfd))) { // Windows encontr un Pixel Format?
		KillGLWin();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.",
			       "ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd)) {		// Podemos aplicar un Pixel Format?
		KillGLWin();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!(hRC = wglCreateContext(hDC))) {			// Tenemos un Rendering Context?
		KillGLWin();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.",
			       "ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!wglMakeCurrent(hDC,hRC)) {					// Activamos el Rendering Context
		KillGLWin();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.",
			       "ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd,SW_SHOW);						// Mostramos la ventana
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Teclado orientado a la ventana
	ResizeGLScene(width, height);					

	if(!SetGL()) {								    // Inicializar nueva ventana GL
		KillGLWin();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									// Exito
}


// Matamos la ventana y la memoria creada por inicializacin
GLvoid OglInit::KillGLWin() {		

	if(fullscreen) {							// Esta Fullscreen?
		ChangeDisplaySettings(NULL,0);			// Cambiamos al Escritorio
		ShowCursor(TRUE);						// Mostramos el Mouse Pointer
	}

	if(hRC) {   								// Tenemos Rendering Context?
		if(!wglMakeCurrent(NULL,NULL)) {		// Podemos liberar el DC y el RC?
			MessageBox(NULL,"Release Of DC And RC Failed.",
				       "SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if(!wglDeleteContext(hRC)) {			// Podemos borrar el RC?
			MessageBox(NULL,"Release Rendering Context Failed.",
				       "SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;								// Poner RC como NULL
	}

	if(hDC && !ReleaseDC(hWnd,hDC)) {			// Podemos liberar el DC
		MessageBox(NULL,"Release Device Context Failed.",
			       "SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC = NULL;								// Ponemos DC como NULL
	}

	if(hWnd && !DestroyWindow(hWnd)) {			// Podemos destruir la ventana?
		MessageBox(NULL,"Could Not Release hWnd.",
			       "SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;							// Ponemos hWnd como NULL
	}
}


/*************** Funciones de configuracin de OGL ***************/
// Modifica el tamao e inicializa de la ventana de rendereo
GLvoid OglInit::ResizeGLScene(GLsizei width, GLsizei height) {
	
	if(height == 0) {								// Previene divisin con cero
		height = 1;									// haciendo a "height" igual a 1
	}

	glViewport(0,0,width,height);					// Resetea el Viewport actual

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccin
	glLoadIdentity();								// Reset The Projection Matrix

	// Calcula el Aspect Ratio de la ventana 
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);						// Selecciona la Matriz de Modelview
	glLoadIdentity();								// Resetea la Matriz de Modelview
}


// Da valores default para OpenGL
int OglInit::SetGL() {			

    //glClearColor(0.35f,0.53f,0.7f,1.0f);
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	glShadeModel(GL_SMOOTH);				// Habilita Smooth Shading
	//glEnable(GL_CULL_FACE);
	
	glClearDepth(1.0f);							// Depth Buffer
	glEnable(GL_DEPTH_TEST);					// Habilita Depth Testing
	glDepthFunc(GL_LEQUAL);						// Tipo de Depth Testing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Calculos de Perspectiva
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

	return TRUE;							// Inicializacion OK
}