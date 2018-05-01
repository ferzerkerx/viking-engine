/**
* @file TextureWriter.h
* @brief Encabezado de la clase TextureWriter
* @date Saturday, October 20, 2007 5:20:31 PM
*/

#ifndef __TEXTUREWRITER__H__
#define __TEXTUREWRITER__H__

#include <fstream>

/**
* @class TextureWriter
* @author Fernando Montes de Oca Cespedes
* @date Saturday, October 20, 2007 4:47:49 PM
* @brief Interfaz que define un escritor para texturas
* decidi no ponerle atributos porque cada header de formato varia
* abre un flujo binario y el destructor lo cierra
*/
class TextureWriter {
protected:
    std::ofstream m_out;

    virtual ~TextureWriter();

public:
    virtual void writeTexture() = 0;

    explicit TextureWriter(const char *file);

};

#endif