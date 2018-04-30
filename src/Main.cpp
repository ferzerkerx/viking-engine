/*!
 *
 *\autor Fernando Montes de Oca C.
 *   	Luis Daniel Rodrguez Palazuelos
 * \date Enero 2007
 *
*/

#include "framework/Framework.h"
#include "Camara.h"
#include "VKSkyDome.h"
#include "framework/sky/SkyBox.h"
#include "framework/model_loaders/MD2Model.h"
#include "framework/loggers/ConsoleEventLogger.h"

#include <GL/glut.h>


using namespace std;

int alto = 768;
int ancho = 1024;
bool wireframe = false;
bool useCamera = false;
bool shouldRenderScreenText = false;
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

void handleMouseMovement(int button, int state, int mouseX, int mouseY);

void myTimer(int i);

void updateScene();

void renderScreenText();

void renderFloor();

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
    glutKeyboardUpFunc(processAsciiKeyEvents);
    glutMouseFunc(handleMouseMovement);

    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

    glutTimerFunc(10, myTimer, 1);

    glutMainLoop();
    shutdownViking();
}


void resize(int width, int height) {
    ancho = width;
    alto = height;
    const float aspectRatio = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30.0, aspectRatio, 0.1F, 1000.0F);

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
        case 'c':
            useCamera = !useCamera;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void handleMouseMovement(int button, int state, int mouseX, int mouseY) {
    if (useCamera) {
        camara.VistaMouse(ancho, alto, mouseX, mouseY);
    }
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

    glTranslatef(0.0F, 0.0F, -5.0F);

    if (vkSkyDome) {
        glPushMatrix();
        glTranslatef(0.0F, -15.0F, 0.0F);
        vkSkyDome->actualiza(camara.getPos());
        glPopMatrix();
    }

    if (skyBox) {
        glPushMatrix();

        skyBox->actualiza(camara.getPos());
        glPopMatrix();
    }

    renderFloor();

    if (knight) {
        glPushMatrix();
        glTranslatef(0.0F, 3.6F, 0.0F);
        glScalef(0.15F, 0.15F, 0.15F);
        knight->renderWithOpenGlCommands();
        glPopMatrix();
    }

    if (shouldRenderScreenText) {
        renderScreenText();
    }

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    glutSwapBuffers();
}

void renderFloor() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, floorTexture[0]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-5.0F, 0.0F, -5.0F);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(45.0F, 0.0F, -5.0F);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(45.0F, 0.0F, 45.0F);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-5.0F, 0.0F, 45.0F);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void renderScreenText() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_TEXTURE_2D);
    if (knight) {
        fuente->glPrint(20, 95, "'n' siguiente frame, 'p' frame anterior", 1, ancho, alto);
    }
    if (vkSkyDome) {
        fuente->glPrint(20, 75, "'g' guardar (test.bmp) ", 1, ancho, alto);
    }
    fuente->glPrint(20, 35, "'f' WireFrame (Toggle)", 1, ancho, alto);
    fuente->glPrint(20, 10, "'c' Camara", 1, ancho, alto);
    glDisable(GL_TEXTURE_2D);
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

        skyBox = new SkyBox(50.0F, 100.0F, 120.0F, skybox_text);
    }

    if (useSkyDome) {
        vkSkyDome = new VKSkyDome(15, 15, 70, sky[0]);
        vkSkyDome->setSunTexture(sun[0]);
    }

    fuente = new Fuente(fontText);
    timer = new Timer();

    knight = new MD2Model("data/modelos/knight.md2", "data/modelos/knight.bmp");

    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

    glShadeModel(GL_SMOOTH);                // Habilita Smooth Shading

    glClearDepth(1.0F);                            // Depth Buffer
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