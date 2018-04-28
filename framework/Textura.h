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

#include <stdio.h>
#include <stdlib.h>

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

        void CrearTextura(unsigned int textureArray[], char * strFileName, int textureID);

        //////////////////////// BMP //////////////////////////////////////
        void CreaBMP(unsigned int textureArray[], char * strFileName, int textureID);
		
        //////////////////////// TGA //////////////////////////////////////
        void CreaTGA(unsigned int textureArray[], char * strFileName, int textureID);
        tImageTGA *CargaTGA(const char *filename);
};

#endif