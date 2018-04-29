/**
* @file TextureWriter.cpp
* @brief Implementacin de la clase TextureWriter
* @date Saturday, October 20, 2007 5:20:37 PM
*/

#include "TextureWriter.h"

/**
* @brief Abre un stream para escribir un archivo
* en forma binaria
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 5:24:55 PM
* @param file Es el nombre del archivo
*/
TextureWriter::TextureWriter(const char *file) {
    m_out.open(file, std::ios::out | std::ios::binary);
    if (!m_out) {
        printf("Error al abrir el archivo para escritura binaria\n");
    }
}


/**
* @brief Cierra el archivo
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 5:24:55 PM
*/
TextureWriter::~TextureWriter() {
    if (m_out) { m_out.close(); }
}