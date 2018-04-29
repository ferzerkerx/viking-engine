#ifndef _DEFS_H
#define _DEFS_H

#include <GL/glut.h>
#include <math.h>			// Header para pow, sqrt
#include <sstream>			// Header para salida de texto

extern int ancho,alto;
static int bits,fs = 0;				// Variables caracteristicas de ventana
static FILE *config;

typedef struct {
    float x;
	float y;
}CIRCULO;

/*************************** Macros y constantes ***************************/
// returns a number ranging from -1.0 to 1.0
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);

#define RAIZ(x)  (x)*(x)

#endif