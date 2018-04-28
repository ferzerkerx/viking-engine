/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Julio 2006
 *
 * Carga texturas para Opengl, BMP y Tga
 *
 *
 ************************************************************************/

#ifndef TEXTURA_H
#define TEXTURA_H

#include <windows.h>		// Header para Windows
#include <stdio.h>
#include <stdlib.h>

#include "OGLInit.h"


#define TGA_RGB		 2		// RGB normal (realmnte BGR)
#define TGA_A		 3		// ALPHA 
#define TGA_RLE		10		// Run-Length Encoded (RLE), compresion

struct tImageTGA {			// Estructura de TGA
	int channels;			// Canales (3 = RGB : 4 = RGBA)
	int sizeX;				// Ancho de la imagen en pixeles
	int sizeY;				// Largo de la imagen en pixeles
	unsigned char *data;	// Image pixel data
};



class Textura {

    public:

		bool hasAlpha;
	   		
		Textura();
		~Textura(){};

        void CrearTextura(UINT textureArray[], LPSTR strFileName, int textureID);

        //////////////////////// BMP //////////////////////////////////////
        void CreaBMP(UINT textureArray[], LPSTR strFileName, int textureID);
		
        //////////////////////// TGA //////////////////////////////////////
        void CreaTGA(UINT textureArray[], LPSTR strFileName, int textureID);
        tImageTGA *CargaTGA(const char *filename);
};

#endif