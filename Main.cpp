/*!
 *
 *\autor Fernando Montes de Oca C.
 *   	Luis Daniel Rodrguez Palazuelos
 * \date Enero 2007
 *
*/

#include "Framework.h"
#include <vector>
#include <cstring>
#include "loggers/ConsoleEventLogger.h"
#include "math/Quaternion.h"
#include "definiciones.h"
#include "Camara.h"
#include "Textura.h"

#include <GL/glut.h>


using namespace std;

int alto = 600;
int ancho = 800;
float cov = 0.0f;


void initViking();
void shutdownViking();


bool	keys[256];			// Arreglo usado para el Teclado
bool	active = true;		// Bandera de actividad de la ventana
bool	fullscreen = true;	// Bandera de Fullscreen
bool    done = false;       // Bandera para salir dl ciclo principal


bool camara, billB, lockFrames = false;
bool wireframe = false;

unsigned int fText[1];
unsigned int piso[1];
unsigned int arbol[1];
unsigned int sky[1];
unsigned int sun[1];
char fps[20], *bbStr="No Billboarding";

CIRCULO circulo;
//Timer *timer;
Textura te;
Camara cam;
//Fuente *fuente;
//VKSkyDome *vksd;
//SkyBox *sb;
EventLogger *g_Log;
//MD2Model *knight;


void DrawGLScene();

void resize(int width, int height) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30.0, ar, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void processKeyEvents(int key, int mouseX, int mouseY) {
    glutPostRedisplay();
}

void myTimer(int i) {
    DrawGLScene();
    glutTimerFunc(10, myTimer, 1);
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Viking Engine");

    initViking();

    glutReshapeFunc(resize);
    glutDisplayFunc(DrawGLScene);
    glutSpecialFunc(processKeyEvents);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


//    glutReshapeFunc(resize);
//    glutSpecialFunc(processKeyEvents);

    glutTimerFunc(10, myTimer, 1);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    glutTimerFunc(10, myTimer, 1);

    glutMainLoop();
    shutdownViking();
}

int leeConfig() {

	config = fopen("config.ini", "r");	 //Abrimos el archivo
	printf("---> config.ini \n");
	if(!config) {
		printf("Imposible encontrar o abrir el archivo config.ini");
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

double GetTickCount(void)
{
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now))
		return 0;
	return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
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
void DrawGLScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	cam.Vista();
//
//	glTranslatef(0.0f,0.0f,-5.0f);

//	if (vksd!=NULL){
//		glPushMatrix();
//			glTranslatef(0.0f,-15.0f,0.0f);
//			vksd->actualiza(cam.getPos());
//		glPopMatrix();
//	}
//
//	if (sb!=NULL){
//		glPushMatrix();
//
//			sb->actualiza(cam.getPos());
//		glPopMatrix();
//	}

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

//	if (knight){
//		glPushMatrix();
//			glTranslatef(0.0f,3.6f,0.0f);
//			glScalef(0.15f,0.15f,0.15f);
//			//knight->render();
//			knight->renderWithOpenGlCommands();
//		glPopMatrix();
//	}



//	/**FUENTE**/
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glEnable(GL_TEXTURE_2D);
//	if (knight){
//		fuente->glPrint(20,95,"'n' siguiente frame, 'p' frame anterior", 1, ancho, alto);
//	}
//	if (vksd){
//		char buff[50];
//		sprintf(buff,"Cover(x aumenta, z decrementa): %f",vksd->getCloudCover());
//		fuente->glPrint(20,55,buff, 1, ancho, alto);
//		fuente->glPrint(20,75,"'g' guardar (test.bmp) ", 1, ancho, alto);
//	}
//	fuente->glPrint(20,35,"'f' WireFrame (Toggle)", 1, ancho, alto);
//	fuente->glPrint(20,10,"'c' Camara", 1, ancho, alto);
//	glDisable(GL_TEXTURE_2D);
//
//	if (wireframe){
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	}
//	else{
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	}
//	/**FUENTE**/
//
//
//	rot++;
}


void manejaTeclado() {
	FN("manejaTeclado()");

	//TODO
//	if(GetKeyState('N') & 0x80) {
//		knight->nextAnimation();
//	}
//	if(GetKeyState('P') & 0x80) {
//		knight->previousAnimation();
//	}
//	//*** Activar camara
//	if(GetKeyState('C') & 0x80) {
//
//		if(camara) {
//			ShowCursor(true);
//			camara = false;
//		} else {
//			ShowCursor(false);
//			camara = true;
//		}
//	}
//
//	if(GetKeyState('F') & 0x80) {
//		if (wireframe){
//			wireframe = false;
//		}
//		else{
//			wireframe = true;
//		}
//	}
//
//	if(GetKeyState('Z') & 0x80) {
//		if (vksd){
//			vksd->decrementaCloudCover();
//		}
//
//	}
//
//	if(GetKeyState('X') & 0x80) {
//		if (vksd){
//			vksd->incrementaCloudCover();
//		}
//
//	}
//
//	if(GetKeyState('G') & 0x80) {
//		if (vksd){
//			LOG(0,"Guardando imagen de nubes");
//			vksd->escribeImagenDeNubes();
//		}
//
//	}
}





bool b_acabo = false;
bool flag = false;


void initViking(){
	g_Log = new ConsoleEventLogger();
	LOG_INIT("TestLog.html");
	FN("initViking()");
	LOG( "El log ha comenzado");

	cam.PonCam(0.0,1.0,10.0,0.0,1.0,9.0,0.0,1.0,0.0);
//
	te.CrearTextura(piso,"data/piso.bmp",0);
//	te.CrearTextura(fText,"data/Fuente/fuente.bmp",0);
//	te.CrearTextura(sky,"data/skydome/image5.bmp",0);
//	te.CrearTextura(sun,"data/sol_mixteco.tga",0);

	//unsigned int skybox_text[6];//FRONT,BACK,LEFT,RIGHT,UP,DOWN
	//te.CrearTextura(skybox_text,"data/skydome/image1.bmp",0);
	//te.CrearTextura(skybox_text,"data/skydome/image2.bmp",1);
	//te.CrearTextura(skybox_text,"data/skydome/image3.bmp",2);
	//te.CrearTextura(skybox_text,"data/skydome/image4.bmp",3);
	//te.CrearTextura(skybox_text,"data/skydome/image5.bmp",4);
	//te.CrearTextura(skybox_text,"data/skydome/image6.bmp",5);

	//sb = new SkyBox(50.0f,100.0f,120.0f,skybox_text);

//	fuente = new Fuente(fText);
//	timer = new Timer();



//    knight = new MD2Model("data/modelos/knight.md2", "data/modelos/knight.bmp");

//	vksd = new VKSkyDome(15,15,70,sky[0]);
//	vksd->setSunTexture(sun[0]);
}


void shutdownViking(){
	FN("shutdownViking()");
	LOG( "Finalizando (destruyendo todo)");
//	delete fuente;
//	delete timer;
//	if (vksd!=NULL){delete vksd;}
//	if (sb!=NULL){delete sb;}
//	if (knight!=NULL){delete knight;}
	LOG_TERM();
	if(g_Log!=NULL){delete g_Log;}
}