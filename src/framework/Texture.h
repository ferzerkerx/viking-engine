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

#include <cstdio>
#include <stdlib.h>

#define TGA_RGB         2        // RGB normal (realmnte BGR)
#define TGA_A         3        // ALPHA
#define TGA_RLE        10        // Run-Length Encoded (RLE), compresion

struct tImageTGA {            // Estructura de TGA
    int channels;            // Canales (3 = RGB : 4 = RGBA)
    int sizeX;                // Ancho de la imagen en pixeles
    int sizeY;                // Largo de la imagen en pixeles
    unsigned char *data;    // Image pixel data
};


class Texture {

public:

    bool hasAlpha;

    Texture();

    ~Texture() {};

    void LoadTexture(unsigned int *textureArray, const char *strFileName, int textureID);

    void LoadBmp(unsigned int *textureArray, const char *strFileName, int textureID);

    void LoadTga(unsigned int *textureArray, const char *strFileName, int textureID);

    tImageTGA *LoadTga(const char *filename);

    void
    ProcessUnCompressedTga(tImageTGA *pImageData, unsigned short width, unsigned short height, unsigned char color_depth,
                           FILE *pFile) const;

    void ProcessCompressedTga(tImageTGA *pImageData, unsigned short width, unsigned short height, unsigned char color_depth,
                              FILE *pFile) const;
};


#endif