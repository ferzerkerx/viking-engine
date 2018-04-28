/*!
 *
 *\autor Fernando Montes de Oca C.
 *   	Luis Daniel Rodrguez Palazuelos
 * \date Enero 2007
 * 
*/

#include "../framework/OGLInit.h"
#include "../framework/Framework.h"
#include "Camara.h" 
#include "../framework/math/Matrix.h"
#include <vector>
#include "VKSkyDome.h"
#include "../framework/sky/SkyBox.h"
#include "../framework/loggers/ConsoleEventLogger.h"
#include "../framework/loggers/TextEventLogger.h"
#include "../framework/loggers/HTMLEventLogger.h"
#include "../framework/math/Quaternion.h"
#include "../framework/model_loaders/MD2Model.h"


using namespace std;

#define ANCHO 800
#define ALTO 600

int alto = 600;
int ancho = 800;
float cov = 0.0f;


Matrix m1, m2, res;
/*prototipos de funciones*/
void testMatrix();
void testQuaternion();
void initViking();
void shutdownViking();

HINSTANCE	hInstance;		// Instancia de la aplicacin

bool	keys[256];			// Arreglo usado para el Teclado
bool	active = TRUE;		// Bandera de actividad de la ventana 
bool	fullscreen = TRUE;	// Bandera de Fullscreen
bool    done = FALSE;       // Bandera para salir dl ciclo principal



bool camara, billB, lockFrames = false;
bool wireframe = false;

unsigned int fText[1];
unsigned int piso[1];
unsigned int arbol[1];
unsigned int sky[1];
unsigned int sun[1];
char fps[20], *bbStr="No Billboarding";

OglInit oi;                 // Objeto para inicializar Opengl
CIRCULO circulo;
Timer *timer;
Textura te;
Camara cam;
Fuente *fuente;
VKSkyDome *vksd;
SkyBox *sb;
EventLogger *g_Log;
MD2Model *knight;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracin de WndProc

int leeConfig() {
	
	config = fopen("config.ini", "r");	 //Abrimos el archivo
	printf("---> config.ini \n");
	if(!config) {
		MessageBox(NULL, "Imposible encontrar o abrir el archivo config.ini", "Error", MB_OK);
		return -1;
	}
	char strWord[255] = {0};
	while(!feof(config)) {
		fscanf(config, "%s", &strWord);	// Obtener cada palabra del archvio
		if(!strcmp(strWord, "Ancho")) {	
			fgets(strWord,50,config);
			ancho = atoi(strWord);
			printf("Ancho: %d \n",ancho);
		}
		if(!strcmp(strWord, "Alto")) {	
			fgets(strWord,50,config);
			alto = atoi(strWord);
			printf("Alto: %d \n",alto);
		}
		if(!strcmp(strWord, "Bits")) {	
			fgets(strWord,50,config);
			bits = atoi(strWord);
			printf("Bits: %d \n",bits);
		}
		if(!strcmp(strWord, "Fullscreen")) {	
			fgets(strWord,50,config);
			fs = atoi(strWord);
			if(fs == 0)
				fullscreen = false;
			else
				fullscreen = true;
			printf("Fullscreen: %d \n",fullscreen);
		}
	}
	fclose(config);

	return 1;
}

//	Define el frame rate de la escena
bool FrameRate(int frame_rate) {
	static float framesPerSecond = 0.0f;
    static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f; // Obtener segundos (ms * .001 = segs) 
	
	if(!lockFrames) {
		++framesPerSecond;
        if(currentTime - lastTime > 1.0f) {
			lastTime = currentTime;
			sprintf(fps, "Frames Per Second: %d", int(framesPerSecond));
			framesPerSecond = 0;
		}
		return true;
	} else {
        // Obtenemos el tiempo transcurrido
		if((currentTime - lastTime) > (1.0f / frame_rate)) {
			sprintf(fps, "Frames Per Second: %d", frame_rate);
			lastTime = currentTime;	// Reseteamos el lastTime	
			framesPerSecond = 0;
			return true;
		}
        return false;
	}
}

void crearCirculo(float k, float r, float h) {
	glBegin(GL_LINES);
		for(int i = 0;i < 180;i++) {
            circulo.x = r * cos((float)i) - h;
			circulo.y = r * sin((float)i) + k;
			glVertex3f(circulo.x + k,circulo.y - h,0);
	
			circulo.x = r * (float)cos(i + 0.1) - h;
			circulo.y = r * (float)sin(i + 0.1) + k;
			glVertex3f(circulo.x + k,circulo.y - h,0);
		}
	glEnd();
}


float rot=0.0;
int DrawGLScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	cam.Vista();
	
	glTranslatef(0.0f,0.0f,-5.0f);

	if (vksd!=NULL){
		glPushMatrix();
			glTranslatef(0.0f,-15.0f,0.0f);
			vksd->actualiza(cam.getPos());
		glPopMatrix();
	}

	if (sb!=NULL){
		glPushMatrix();
			
			sb->actualiza(cam.getPos());
		glPopMatrix();
	}
	
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, piso[0]);
		glBegin(GL_POLYGON);			
			glTexCoord2f(0.0,1.0); glVertex3f(-5.0, 0.0, -5.0);
			glTexCoord2f(1.0,1.0); glVertex3f(45.0, 0.0, -5.0);
			glTexCoord2f(1.0,0.0); glVertex3f(45.0, 0.0, 45.0);
			glTexCoord2f(0.0,0.0); glVertex3f(-5.0, 0.0, 45.0);
		glEnd();		
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	if (knight){
		glPushMatrix();
			glTranslatef(0.0f,3.6f,0.0f);
			glScalef(0.15f,0.15f,0.15f);
			//knight->render();
			knight->renderWithOpenGlCommands();
		glPopMatrix();
	}
	

	
	/**FUENTE**/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	if (knight){
		fuente->glPrint(20,95,"'n' siguiente frame, 'p' frame anterior", 1, ancho, alto);
	}
	if (vksd){
		char buff[50];
		sprintf(buff,"Cover(x aumenta, z decrementa): %f",vksd->getCloudCover());
		fuente->glPrint(20,55,buff, 1, ancho, alto);
		fuente->glPrint(20,75,"'g' guardar (test.bmp) ", 1, ancho, alto);
	}
	fuente->glPrint(20,35,"'f' WireFrame (Toggle)", 1, ancho, alto);
	fuente->glPrint(20,10,"'c' Camara", 1, ancho, alto);
	glDisable(GL_TEXTURE_2D);

	if (wireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	/**FUENTE**/
	

	rot++;
	
	return TRUE;										
}


void manejaTeclado() {
	FN("manejaTeclado()");

	if(GetKeyState('N') & 0x80) {
		knight->nextAnimation();
	}
	if(GetKeyState('P') & 0x80) {
		knight->previousAnimation();
	}
	//*** Activar camara
	if(GetKeyState('C') & 0x80) {

		if(camara) {
			ShowCursor(true);
			camara = false;
		} else {
			ShowCursor(false);
			camara = true;
		}
	}

	if(GetKeyState('F') & 0x80) {
		if (wireframe){
			wireframe = false;
		}
		else{
			wireframe = true;
		}
	}

	if(GetKeyState('Z') & 0x80) {
		if (vksd){
			vksd->decrementaCloudCover();
		}
	
	}
	
	if(GetKeyState('X') & 0x80) {
		if (vksd){
			vksd->incrementaCloudCover();
		}
	
	}

	if(GetKeyState('G') & 0x80) {
		if (vksd){
			LOG(0,"Guardando imagen de nubes");
			vksd->escribeImagenDeNubes();
		}
	
	}
}


/*	Este cdigo crea una ventana OpenGL.  Parametros:   					*
 *	title			- Titulo que aparece en la ventana      				*
 *	width			- Ancho de la ventana									*
 *	height			- Alto de la ventana									*
 *	bits			- Nmero de bits de color (8/16/24/32)					*
 *	fullscreenflag	- Usar modo Fullscreen (TRUE) or Ventana (FALSE)	    */
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {

	WNDCLASS	wc;						// Estructura de Windows Class
	DWORD		dwExStyle;				// Estilo de Ventana Extendida
	DWORD		dwStyle;				// Estilo de Ventana
	RECT		WindowRect;				// Jala valores de la ventana
	WindowRect.left = (long)0;			// Valor izquierdo en 0
	WindowRect.right = (long)width;		// Valor derecho como Width (Ancho)
	WindowRect.top = (long)0;			// Valor superior en 0
	WindowRect.bottom = (long)height;	// Valor inferior como Height (Alto)

	fullscreen = fullscreenflag;		// Asignamos Fullscreen global

	hInstance		 = GetModuleHandle(NULL);				// Jala Instancia para nuestra Ventana
	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja Size y DC
	wc.lpfnWndProc	 = (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra	 = 0;									// No datos de Ventana Extra
	wc.cbWndExtra	 = 0;									// No datos de Ventana Extra
	wc.hInstance	 = hInstance;							// Asignar Instancia
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);			// Carga el Icono default
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);			// Carga cursor de flecha
	wc.hbrBackground = NULL;								// No se requiere Background
	wc.lpszMenuName	 = NULL;								// No se requiere Menu
	wc.lpszClassName = "OpenGL";							// Asignar el nombre de la Clase

	if(!RegisterClass(&wc)) {			// Checar el registro de la Clase
		MessageBox(NULL,"Failed To Register The Window Class.",
			       "ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;					
	}
	
	if(fullscreen) {											// Fullscreen Mode?
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Asegurar memoria
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Tamao de Devmode
		dmScreenSettings.dmPelsWidth	= width;				// Selecciona Width
		dmScreenSettings.dmPelsHeight	= height;				// Selecciona Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selecciona Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Asignar el modo y aplicarlo. 
		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!= DISP_CHANGE_SUCCESSFUL) {
			// Si falla modo, dar 2 opciones.  Quitar o usar modo ventana.
			if(MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
				          "OpenGL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES) {
				fullscreen = FALSE;		// Modo ventana
			}
			else {
				// Mensaje de cerrar programa 
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}

	if(fullscreen) {							// Estamos en Fullscreen Mode?
		dwExStyle = WS_EX_APPWINDOW;			// Estilo Window Extended
		dwStyle = WS_POPUP;						// Estilo Windows
		//ShowCursor(FALSE);			    		// Ocultar Mouse Pointer
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;		// Estilo Window Extended
		dwStyle = WS_OVERLAPPEDWINDOW;						// Estilo Windows
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Ajustar tamao ventana

	// Crear la Ventana 
	if(!(oi.hWnd=CreateWindowEx(dwExStyle,						// Estilo Extended
 							 "OpenGL",							// Nombre de la Clase
							 title,								// Ttulo de la Ventana
							 dwStyle |							// Estilo de Ventana definido
							 WS_CLIPSIBLINGS |					// Estilo de Ventana requerido
							 WS_CLIPCHILDREN,					// Estilo de Ventana requerido
							 0, 0,								// Posicin de Ventana
							 WindowRect.right-WindowRect.left,	// Calcula Ancho de Ventana
							 WindowRect.bottom-WindowRect.top,	// Calcula Alto de Ventana
							 NULL,								// No Parent Window
							 NULL,								// No Menu
							 hInstance,							// Instancia
							 NULL)))							// No pasar nada a WM_CREATE
	{
		oi.KillGLWin();
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	oi.Init(width,height,bits);                 // Inicializacin de Opengl
	return TRUE;								
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	/*
	 * HWND	hWnd	    // Manejador de esta Ventana
	 * UINT	uMsg	    // Mensaje para esta Ventana
	 * WPARAM wParam	// Informacin de Mensaje adicional
	 * LPARAM lParam    // Informacin de Mensaje adicional
	 */
	//PAINTSTRUCT ps;
	
	switch(uMsg) {									// Checa Mensajes de la Ventana
		case WM_ACTIVATE: {   						// Checa Mensaje de Ventana activa
			if(!HIWORD(wParam)) {					// Checa estado de minimizacin
				active = TRUE;						// El programa esta activo 
			}
			else {
				active = FALSE;						// El programa no esta activo
			}

			return 0;								// Regresa al ciclo del mensaje
		}

		case WM_SYSCOMMAND: {
			switch(wParam) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		}

		case WM_CLOSE: {							// Recibimos mensaje de Cerrar?
			PostQuitMessage(0);						// Mandar mensaje de Quit
			return 0;								
		}

		case WM_KEYDOWN: {							// Ha sido presionada una tecla?
			keys[wParam] = TRUE;					// Marcala como TRUE
			return 0;								
		}

		case WM_KEYUP: {							// Ha sido liberada una tecla?
			keys[wParam] = FALSE;					// Marcala como FALSE
			return 0;								
		}

		case WM_SIZE: {								// Redimensionar la ventana OGL
			oi.ResizeGLScene(LOWORD(lParam),HIWORD(lParam));
			return 0;								
		}
        
		case WM_MOUSEMOVE: {
			int flags = (int)wParam; // "flags" contains flags that say whether certain
		   					    // virtual keys (such as the CTRL key) are being pushed or not
			//xPos = LOWORD(lParam);  // horizontal (the X position) of the cursor
			//yPos = HIWORD(lParam);  // vertical (the Y position) of the cursor
			
			return 0;
		}
						   
		case WM_LBUTTONDOWN: {
			
            return 0;
		}

		case WM_LBUTTONUP: {
			
            return 0;

		}
	}

	// Pasamos todos los mensajes no manejados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

bool b_acabo = false;
bool flag = false;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	/*
	 * HINSTANCE hInstance			// Instancia
	 * HINSTANCE hPrevInstance		// Instancia previa 
	 * LPSTR lpCmdLine  			// Parmetros de Command Line
	 * int nCmdShow     			// Window Show State
	 */
	MSG		msg;					// Estructura de Mensajes de Windows
	
	
	// Preguntar si queremos fullscreen
	if(MessageBox(NULL,"Quieres desplegar Pantalla Completa (Fullscreen)?",
		          "Pantalla Completa?",MB_YESNO|MB_ICONQUESTION)==IDNO) {
		fullscreen = FALSE;				
	}

	// Creamos nuestra ventana OpenGL
	if(!CreateGLWindow("OpenGL Base",ANCHO,ALTO,32,fullscreen)) {
		return 0;				    // Cerrar si la ventana no fue creada
	}
	
	initViking();
	
	while(!done) {									// Ciclo principal (done = FALSE)
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {	// Hay mensaje esperando?
			if(msg.message == WM_QUIT) {			// Recibimos mensaje de Quit?
				done = TRUE;					    // Si?, entonces done = TRUE
			}
			else {									// Si no, manejar los Window Messages
    			TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else {										// Si no hay mensajes

			// Dibujamos la escena. Checar ESC y Quit Messages de DrawGLScene()
			if((active && !DrawGLScene()) || keys[VK_ESCAPE]) {	
				done = TRUE;					    // ESC o Quit de DrawGLScene
			}
			// Dibujamos la escena. Checar ESC y Quit Messages de DrawGLScene()
			if(keys[VK_ESCAPE]) {	
				done = TRUE;					    // ESC o Quit de DrawGLScene	
			}
			else {									// Actualizar pantalla				
				manejaTeclado();

				if(FrameRate(60)) {
					if(camara)
						cam.Actualizar();
					
					SwapBuffers(oi.hDC);				// Swap Buffers (Double Buffering)
				}
			}
		}

	LOG_UPDATE();
	}

	
	shutdownViking();
    oi.KillGLWin();									// Matar la ventana
	return (int)(msg.wParam);							// Salir del programa
}

void testMatrix(){
	
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	m1.addVector(v);
	v.clear();
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	m1.addVector(v);
	v.clear();
	v.push_back(7);
	v.push_back(8);
	m1.addRengl(v);

	int data[][4] = {{2,-1,3,0},{4,-2,7,0},{-3,-4,1,5},{6,-6,8,0}};
	
	m1.setData(&data[0][0],4,4);
	m2.setData(&data[0][0],4,4);
	//res = m1 + m2;
	int data2[][3] = {{2,3,4},{5,6,7},{8,9,1}};
	m2.setData(&data2[0][0],3,3);
	//int data2[][2] = {{2,3},{5,6}};
	//m2.setData(&data2[0][0],2,2);
	//m1 *= m2;
	printf("determinate m2 %d", m2.determinante());
}

void initViking(){
	//g_Log = new ConsoleEventLogger();
	//g_Log = new TextEventLogger();
	g_Log = new HTMLEventLogger();
	LOG_INIT("TestLog.html");
	FN("initViking()");
	LOG( LOG_BOLD  | LOG_COLOR_DK_RED, "El log ha comenzado");

	cam.PonCam(0.0,1.0,10.0,0.0,1.0,9.0,0.0,1.0,0.0);
	
	te.CrearTextura(piso,"data/piso.bmp",0);
	te.CrearTextura(fText,"data/Fuente/fuente.bmp",0);
	te.CrearTextura(sky,"data/skydome/image5.bmp",0);
	te.CrearTextura(sun,"data/sol_mixteco.tga",0);

	//unsigned int skybox_text[6];//FRONT,BACK,LEFT,RIGHT,UP,DOWN
	//te.CrearTextura(skybox_text,"data/skydome/image1.bmp",0);
	//te.CrearTextura(skybox_text,"data/skydome/image2.bmp",1);
	//te.CrearTextura(skybox_text,"data/skydome/image3.bmp",2);
	//te.CrearTextura(skybox_text,"data/skydome/image4.bmp",3);
	//te.CrearTextura(skybox_text,"data/skydome/image5.bmp",4);
	//te.CrearTextura(skybox_text,"data/skydome/image6.bmp",5);

	//sb = new SkyBox(50.0f,100.0f,120.0f,skybox_text);

	fuente = new Fuente(fText);
	timer = new Timer();

	
	
    knight = new MD2Model("data/modelos/knight.md2", "data/modelos/knight.bmp");
		
	vksd = new VKSkyDome(15,15,70,sky[0]);
	vksd->setSunTexture(sun[0]);
	//testMatrix();	
	testQuaternion();
}


void shutdownViking(){
	FN("shutdownViking()");
	LOG( LOG_BOLD  | LOG_COLOR_DK_RED, "Finalizando (destruyendo todo)");
	delete fuente;
	delete timer;
	if (vksd!=NULL){delete vksd;}
	if (sb!=NULL){delete sb;}
	if (knight!=NULL){delete knight;}
	LOG_TERM();	
	if(g_Log!=NULL){delete g_Log;}
}


void testQuaternion(){
	FN("testQuaternion()");
	/*Quaternion q1(1.0f,2.0f,3.0f,4.0f);
	Quaternion q2(4.0f,3.0f,2.0f,1.0f);*/
	Quaternion q1(0.966f,0.0f,0.0f,0.259f);
	Quaternion q2(0.866f,0.0f,0.0f,0.5f);

	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1 = %s, q2 = %s" ,q1.toString(),q2.toString());
	Quaternion q3 =  q1*q2;
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1*q2 = %s,  %d",q3.toString(),q3.isUnitario());
	q3 =  q1+q2;
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1+q2 = %s",q3.toString());
	q3 =  q1-q2;
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1-q2 = %s",q3.toString());
	q3 =  q1.normaliza();
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"Norm(q1) = %s, |Norm(q1)| = %f, %d ",q3.toString(), q3.getMagnitud(),q3.isUnitario());
	q3 =  q1.getConjugado();
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"Conj(q1) = %s",q3.toString());
	Quaternion q4 =  q1.getInversa();
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1^-1 = %s",q4.toString());
	q3 =  q1*q4;
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"q1^-1*q = %s",q3.toString());
	q3 = q1.SLERP(q1,q2,0.3f);
	LOG(LOG_BOLD | LOG_COLOR_DK_BLUE,"SLERP(q1,q2) = %s",q3.toString());

	Quaternion q5(1.0f,2.0f,3.0f,4.0f);
	q5 += q3;
	q5 -= q3;
	q5 *= q3;
	q5 = q5 * 5.0f;
	q5 *= 5.0f;
	q1 == q2;
	q5 = q1;

}