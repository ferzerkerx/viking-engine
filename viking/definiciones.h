#ifndef _DEFS_H
#define _DEFS_H

#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN    

#include <GL/glut.h>
#include <math.h>			// Header para pow, sqrt
#include <sstream>			// Header para salida de texto

#define KEY_DOWN(vk_code)  ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) // Sincronizacin de
#define KEY_UP(vk_code)    ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1) // teclas

//static HINSTANCE	hInstance;		// Instancia de la aplicacin

//static bool	keys[256];			    // Arreglo usado para el Teclado
//static bool	active = true;		    // Bandera de actividad de la ventana 
extern int ancho,alto;              
static int bits,fs = 0;				// Variables caracteristicas de ventana
//static bool	fullscreen = false;	    // Bandera de Fullscreen
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