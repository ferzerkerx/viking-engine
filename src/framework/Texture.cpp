/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Julio 2006
 *
 * Carga texturas para Opengl, BMP, Jpg y Tga
 *
 *
 ************************************************************************/

#include <cstring>
#include <stdexcept>
#include "Texture.h"
#include "GL/glut.h"


Texture::Texture() {
    hasAlpha = false;
}

void Texture::LoadTexture(unsigned int *textureArray, const char *strFileName, int textureID) {
    if (!strFileName) {
        throw std::invalid_argument("Empty filename");
    }

    const char *file_extension = strrchr(strFileName, '.');
    if (file_extension[0]) {
        file_extension++;
    }

    if (!strcmp(file_extension, "bmp")) {
        LoadBmp(textureArray, strFileName, textureID);
        hasAlpha = false;
        printf("Creating texture: %s \n", strFileName);
    } else if (!strcmp(file_extension, "tga")) {
        LoadTga(textureArray, strFileName, textureID);
        hasAlpha = true;
        printf("Creating texture: %s \n", strFileName);
    }
}

void Texture::LoadBmp(unsigned int *textureArray, const char *strFileName, int textureID) {
    int width;
    int height;

    unsigned char header[54]; // 54-bytes header
    int imageSize;   // = width*height*3
    unsigned char *data;

    FILE *file = fopen(strFileName, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        throw std::invalid_argument("File not found");
    }

    if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
        throw std::invalid_argument("Not a correct BMP file\n");
    }
    if (header[0] != 'B' || header[1] != 'M') {
        throw std::invalid_argument("Not a correct BMP file\n");
    }
    // Read ints from the byte array
    imageSize = *(int *) &(header[0x22]);
    width = *(int *) &(header[0x12]);
    height = *(int *) &(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    data = new unsigned char[imageSize];

    fread(data, 1, static_cast<size_t>(imageSize), file);

    fclose(file);

    glGenTextures(1, &textureArray[textureID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    free(data);
}

void Texture::LoadTga(unsigned int *textureArray, const char *strFileName, int textureID) {

    tImageTGA *pImage = LoadTga(strFileName);

    if (pImage == nullptr) {
        return;
    }

    glGenTextures(1, &textureArray[textureID]);
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

    int textureType = GL_RGB;

    if (pImage->channels == 4) {
        textureType = GL_RGBA;
    }

    gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX,
                      pImage->sizeY, static_cast<GLenum>(textureType), GL_UNSIGNED_BYTE, pImage->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] pImage->data;

    free(pImage);
}

tImageTGA *Texture::LoadTga(const char *filename) {

    tImageTGA *pImageData = nullptr;
    unsigned short width = 0;
    unsigned short height = 0;
    unsigned char length = 0;
    unsigned char imageType = 0;
    unsigned char color_depth = 0;
    FILE *pFile = nullptr;

    if ((pFile = fopen(filename, "rb")) == nullptr) {
        printf("TGA: %s \n", filename);
        throw std::invalid_argument("File not found");
    }

    pImageData = (tImageTGA *) malloc(sizeof(tImageTGA));

    fread(&length, sizeof(unsigned char), 1, pFile);
    fseek(pFile, 1, SEEK_CUR);
    fread(&imageType, sizeof(unsigned char), 1, pFile);
    fseek(pFile, 9, SEEK_CUR);

    fread(&width, sizeof(unsigned short), 1, pFile);
    fread(&height, sizeof(unsigned short), 1, pFile);
    fread(&color_depth, sizeof(unsigned char), 1, pFile);

    fseek(pFile, length + 1, SEEK_CUR);

    if (color_depth != 32 && color_depth != 24 && color_depth != 15) {
        throw std::invalid_argument("Color depth not supported for image only 32, 24 and 16");
    }

    bool is_compressed_image = imageType == TGA_RLE;
    if (!is_compressed_image) {
        ProcessUnCompressedTga(pImageData, width, height, color_depth, pFile);
    } else {
        ProcessCompressedTga(pImageData, width, height, color_depth, pFile);

    }

    fclose(pFile);

    return pImageData;
}

void Texture::ProcessCompressedTga(tImageTGA *pImageData, unsigned short width, unsigned short height,
                                   unsigned char color_depth, FILE *pFile) const {

    int channels = 0;
    int stride = 0;
    int i = 0;
    unsigned char rleID = 0;
    int colorsRead = 0;
    channels = color_depth / 8;
    stride = channels * width;

    pImageData->data = new unsigned char[stride * height];
    auto *pColors = new unsigned char[channels];

    while (i < width * height) {
            fread(&rleID, sizeof(unsigned char), 1, pFile);

            if (rleID < 128) {
                rleID++;
                while (rleID) {
                    fread(pColors, sizeof(unsigned char) * channels, 1, pFile);

                    pImageData->data[colorsRead + 0] = pColors[2];
                    pImageData->data[colorsRead + 1] = pColors[1];
                    pImageData->data[colorsRead + 2] = pColors[0];

                    if (color_depth == 32)
                        pImageData->data[colorsRead + 3] = pColors[3];

                    i++;
                    rleID--;
                    colorsRead += channels;
                }
            } else {
                rleID -= 127;

                fread(pColors, sizeof(unsigned char) * channels, 1, pFile);

                while (rleID) {
                    pImageData->data[colorsRead + 0] = pColors[2];
                    pImageData->data[colorsRead + 1] = pColors[1];
                    pImageData->data[colorsRead + 2] = pColors[0];

                    if (color_depth == 32)
                        pImageData->data[colorsRead + 3] = pColors[3];

                    i++;
                    rleID--;
                    colorsRead += channels;
                }
            }
        }

    delete[] pColors;

    pImageData->channels = channels;
    pImageData->sizeX = width;
    pImageData->sizeY = height;
}

void Texture::ProcessUnCompressedTga(tImageTGA *pImageData, unsigned short width, unsigned short height,
                                     unsigned char color_depth, FILE *pFile) const {
    int channels = 0;
    int stride = 0;
    int i = 0;

    if (color_depth == 24 || color_depth == 32) {
        channels = color_depth / 8;
        stride = channels * width;
        pImageData->data = new unsigned char[stride * height];

        for (int y = 0; y < height; y++) {
            unsigned char *pLine = &(pImageData->data[stride * y]);
            fread(pLine, static_cast<size_t>(stride), 1, pFile);

            for (i = 0; i < stride; i += channels) {
                int temp = pLine[i];
                pLine[i] = pLine[i + 2];
                pLine[i + 2] = static_cast<unsigned char>(temp);
            }
        }
    } else if (color_depth == 16) {
        unsigned short pixels = 0;
        int r = 0;
        int g = 0;
        int b = 0;

        channels = 3;
        stride = channels * width;
        pImageData->data = new unsigned char[stride * height];

        for (int i = 0; i < width * height; i++) {
            fread(&pixels, sizeof(unsigned short), 1, pFile);
            b = (pixels & 0x1f) << 3;
            g = ((pixels >> 5) & 0x1f) << 3;
            r = ((pixels >> 10) & 0x1f) << 3;

            pImageData->data[i * 3 + 0] = static_cast<unsigned char>(r);
            pImageData->data[i * 3 + 1] = static_cast<unsigned char>(g);
            pImageData->data[i * 3 + 2] = static_cast<unsigned char>(b);
        }
    }

    pImageData->channels = channels;
    pImageData->sizeX = width;
    pImageData->sizeY = height;

}