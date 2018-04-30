/*!
\file Fuente.cpp
\brief Implementacion de las funciones de la clase Fuente
*/
#include "Fuente.h"
#include <GL/gl.h>
#include <cstring>


/*Constructor*/
Fuente::Fuente(unsigned int *text) {
    textura = text;

    /*coordenadas del caracter en cuestin*/
    float cx;
    float cy;

    int i = 0;
    /*generamos las listas*/
    base = glGenLists(256);
    /*seleccionamos la textura a utilizar*/
    glBindTexture(GL_TEXTURE_2D, *textura);

    for (i = 0; i < 256; i++) {

        /*asignamos las coordenadas(x,y)de nuestro caracter*/
        cx = float(i % 16) / 16.0f;
        cy = float(i / 16) / 16.0f;

        /*comenzamos a llenar las listas*/
        glNewList(base + i, GL_COMPILE);
        /*generamos un polgono para cada caracter*/
        glBegin(GL_QUADS);
        /*abajo izquierda*/
        glTexCoord2f(cx, 1.0f - cy - 0.0625f);
        glVertex2i(0, 0);
        /*abajo derecha*/
        glTexCoord2f(cx + 0.0625f, 1.0f - cy - 0.0625f);
        glVertex2i(16, 0);
        /*arriba derecha*/
        glTexCoord2f(cx + 0.0625f, 1.0f - cy);
        glVertex2i(16, 16);
        /*arriba izquierda*/
        glTexCoord2f(cx, 1.0f - cy);
        glVertex2i(0, 16);
        glEnd();
        /*nos movemos a la derecha*/
        glTranslatef(11, 0, 0);
        /*fin de la lista*/
        glEndList();
    }//for
}

/*Destructor*/
Fuente::~Fuente() {
    glDeleteLists(base, 256);
    textura = nullptr;
}

void Fuente::glPrint(float x, float y, const char *string, int set, int ancho, int alto) {
    /*Establecemos el color del alpha (todos menos el color de la fuente)*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_BLEND);
    /*Seleccionamos la textura donde est la fuente*/
    glBindTexture(GL_TEXTURE_2D, *textura);
    /*Deshabilitamos la profundidad*/
    glDisable(GL_DEPTH_TEST);
    /*cambiamos a matriz de proyeccin*/
    glMatrixMode(GL_PROJECTION);
    /*hacemos push de la matriz*/
    glPushMatrix();
    /*reseteamos*/
    glLoadIdentity();
    /*nos cambiamos a modo ortogonal*/
    glOrtho(0, ancho, 0, alto, -1, 1);
    /*escogemos la matriz de vista de modelo*/
    glMatrixMode(GL_MODELVIEW);
    /*hacemos push de la matriz*/
    glPushMatrix();
    /*reseteamos*/
    glLoadIdentity();
    /*nos colocamos en la posicion especificada (x,y)*/
    glTranslated(x, y, 0);
    /*escogemos las listas del set especificado*/
    glListBase(base - 32 + (128 * set));
    /*Escribimos en pantalla*/
    glCallLists(static_cast<GLsizei>(strlen(string)), GL_BYTE, string);
    /*escogemos la matriz de proyeccin*/
    glMatrixMode(GL_PROJECTION);
    /*regresamos a la matriz anterior*/
    glPopMatrix();
    /*escogemos la matriz de vista de modelos*/
    glMatrixMode(GL_MODELVIEW);
    /*regresamos a la matriz anterior*/
    glPopMatrix();
    /*habilitamos la profundidad*/
    glEnable(GL_DEPTH_TEST);
    /*deshabilitamos el alpha*/
    glDisable(GL_BLEND);
}
