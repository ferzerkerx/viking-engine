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
#include "Camara.h"
#include "sky/SkyBox.h"
#include "VKSkyDome.h"
#include "model_loaders/MD2Model.h"

#include <GL/glut.h>


using namespace std;

int alto = 600;
int ancho = 800;
bool wireframe = false;
bool useSkyDome = true;
bool useSkybox = false;
float currentRotation = 0.0;

unsigned int fontText[1];
unsigned int floorTexture[1];
unsigned int sky[1];
unsigned int sun[1];

Timer *timer;
Textura textura;
Camara camara;
Fuente *fuente;
VKSkyDome *vkSkyDome;
SkyBox *skyBox;
EventLogger *g_Log;
MD2Model *knight;

void initViking();

void shutdownViking();

void renderScene();

void resize(int width, int height);

void processNonAsciiKeyEvents(int key, int mouseX, int mouseY);

void processAsciiKeyEvents(unsigned char key, int mouseX, int mouseY);

void myTimer(int i);

void updateScene();

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Viking Engine");

    initViking();

    glutReshapeFunc(resize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(processNonAsciiKeyEvents);
    glutKeyboardUpFunc(processAsciiKeyEvents);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutTimerFunc(10, myTimer, 1);

    glutMainLoop();
    shutdownViking();
}


void resize(int width, int height) {
    const float aspectRatio = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30.0, aspectRatio, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void processAsciiKeyEvents(unsigned char key, int mouseX, int mouseY) {
    FN("processAsciiKeyEvents()");
    switch (key) {
        case 'n':
            if (knight) {
                knight->nextAnimation();
            }
            break;
        case 'p':
            if (knight) {
                knight->previousAnimation();
            }
            break;
        case 'f':
            wireframe = !wireframe;
            break;
        case 'z':
            if (vkSkyDome) {
                vkSkyDome->decrementaCloudCover();
            }
            break;
        case 'x':
            if (vkSkyDome) {
                vkSkyDome->incrementaCloudCover();
            }
            break;
        case 'g':
            if (vkSkyDome) {
                LOG(0, "Guardando imagen de nubes");
                vkSkyDome->escribeImagenDeNubes();
            }
            break;
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

    }

    glutPostRedisplay();
}

void processNonAsciiKeyEvents(int key, int mouseX, int mouseY) {
    FN("processNonAsciiKeyEvents()");
    camara.ChecarMov(key);
    glutPostRedisplay();
}

void myTimer(int i) {
    updateScene();
}

void updateScene() {
    camara.Actualizar();
    currentRotation++;
    glutTimerFunc(10, myTimer, 1);
    glutPostRedisplay();
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camara.Vista();

    glTranslatef(0.0f, 0.0f, -5.0f);

    if (vkSkyDome) {
        glPushMatrix();
        glTranslatef(0.0f, -15.0f, 0.0f);
        vkSkyDome->actualiza(camara.getPos());
        glPopMatrix();
    }

    if (skyBox) {
        glPushMatrix();

        skyBox->actualiza(camara.getPos());
        glPopMatrix();
    }

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, floorTexture[0]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(45.0f, 0.0f, -5.0f);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(45.0f, 0.0f, 45.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-5.0f, 0.0f, 45.0f);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    if (knight) {
        glPushMatrix();
        glTranslatef(0.0f, 3.6f, 0.0f);
        glScalef(0.15f, 0.15f, 0.15f);
        knight->renderWithOpenGlCommands();
        glPopMatrix();
    }

    /**FUENTE**/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
    if (knight) {
        fuente->glPrint(20, 95, "'n' siguiente frame, 'p' frame anterior", 1, ancho, alto);
    }
    if (vkSkyDome) {
        char buff[50];
        sprintf(buff, "Cover(x aumenta, z decrementa): %f", vkSkyDome->getCloudCover());
        fuente->glPrint(20, 55, buff, 1, ancho, alto);
        fuente->glPrint(20, 75, "'g' guardar (test.bmp) ", 1, ancho, alto);
    }
    fuente->glPrint(20, 35, "'f' WireFrame (Toggle)", 1, ancho, alto);
    fuente->glPrint(20, 10, "'c' Camara", 1, ancho, alto);
    glDisable(GL_TEXTURE_2D);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    /**FUENTE**/

    glutSwapBuffers();
}


void initViking() {
    g_Log = new ConsoleEventLogger();
    LOG_INIT("TestLog.html");
    FN("initViking()");
    LOG("El log ha comenzado");

    camara.PonCam(0.0, 1.0, 10.0, 0.0, 1.0, 9.0, 0.0, 1.0, 0.0);

    textura.CrearTextura(floorTexture, "data/piso.bmp", 0);
    textura.CrearTextura(fontText, "data/Fuente/fuente.bmp", 0);
    textura.CrearTextura(sky, "data/skydome/image5.bmp", 0);
    textura.CrearTextura(sun, "data/sol_mixteco.tga", 0);

    if (useSkybox) {
        unsigned int skybox_text[6];//FRONT,BACK,LEFT,RIGHT,UP,DOWN
        textura.CrearTextura(skybox_text, "data/skydome/image1.bmp", 0);
        textura.CrearTextura(skybox_text, "data/skydome/image2.bmp", 1);
        textura.CrearTextura(skybox_text, "data/skydome/image3.bmp", 2);
        textura.CrearTextura(skybox_text, "data/skydome/image4.bmp", 3);
        textura.CrearTextura(skybox_text, "data/skydome/image5.bmp", 4);
        textura.CrearTextura(skybox_text, "data/skydome/image6.bmp", 5);

        skyBox = new SkyBox(50.0f, 100.0f, 120.0f, skybox_text);
    }

    if (useSkyDome) {
        vkSkyDome = new VKSkyDome(15, 15, 70, sky[0]);
        vkSkyDome->setSunTexture(sun[0]);
    }

    fuente = new Fuente(fontText);
    timer = new Timer();

    knight = new MD2Model("data/modelos/knight.md2", "data/modelos/knight.bmp");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glShadeModel(GL_SMOOTH);                // Habilita Smooth Shading

    glClearDepth(1.0f);                            // Depth Buffer
    glEnable(GL_DEPTH_TEST);                    // Habilita Depth Testing
    glDepthFunc(GL_LEQUAL);                        // Tipo de Depth Testing
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Calculos de Perspectiva
}


void shutdownViking() {
    FN("shutdownViking()");
    LOG("Finalizando (destruyendo todo)");
    delete fuente;
    delete timer;
    delete vkSkyDome;
    delete skyBox;
    delete knight;
    LOG_TERM();
    delete g_Log;
}