/*!
\file Fuente.cpp
\brief Implementacion de las funciones de la clase Fuente
*/
#include "Fuente.h"
#include <GL/gl.h>
#include <cstring>


Fuente::Fuente(unsigned int *text) : textura(text) {
    float cx;
    float cy;

    int i = 0;
    /*generamos las listas*/
    base = glGenLists(256);
    glBindTexture(GL_TEXTURE_2D, *textura);

    for (i = 0; i < 256; i++) {
        /*asignamos las coordenadas(x_,y_)de nuestro caracter*/
        cx = float(i % 16) / 16.0F;
        cy = float(i / 16) / 16.0F;

        /*comenzamos a llenar las listas*/
        glNewList(base + i, GL_COMPILE);
        /*generamos un polgono para cada caracter*/
        glBegin(GL_QUADS);
        /*abajo izquierda*/
        glTexCoord2f(cx, 1.0F - cy - 0.0625F);
        glVertex2i(0, 0);
        /*abajo derecha*/
        glTexCoord2f(cx + 0.0625F, 1.0F - cy - 0.0625F);
        glVertex2i(16, 0);
        /*arriba derecha*/
        glTexCoord2f(cx + 0.0625F, 1.0F - cy);
        glVertex2i(16, 16);
        /*arriba izquierda*/
        glTexCoord2f(cx, 1.0F - cy);
        glVertex2i(0, 16);
        glEnd();
        /*nos movemos a la derecha*/
        glTranslatef(11, 0, 0);
        /*fin de la lista*/
        glEndList();
    }
}

Fuente::~Fuente() {
    glDeleteLists(base, 256);
    textura = nullptr;
}

void Fuente::glPrint(float x, float y, const char *string, int set, int ancho, int alto) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, *textura);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, ancho, 0, alto, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x, y, 0);
    /*escogemos las listas del set especificado*/
    glListBase(base - 32 + (128 * set));
    /*Escribimos en pantalla*/
    glCallLists(static_cast<GLsizei>(strlen(string)), GL_BYTE, string);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
