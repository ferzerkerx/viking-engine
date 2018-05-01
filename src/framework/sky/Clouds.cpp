/**
* @file Clouds.cpp
* @brief Implementacin de la clase Clouds
* @date Friday, October 19, 2007 5:18:12 PM
*/
#include <GL/glut.h>
#include "Clouds.h"


/**
*@brief Constructor de las nubes, inicializa los factores para generar la
*textura de nubes(cobertura, delgadez, numero de octavos,tamao de la textura)
* por default
*/
Clouds::Clouds() {
    cover_ = 45.0F;
    sharpness_ = 0.987F;
    num_octaves_ = 4;
    m_texture = -1;

    text_width_ = 384;
    text_height_ = 384;

}


/**
* @brief Constructor de las nubes, inicializa los factores para generar la
* textura de nubes con parametros especificos
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 5:07:08 PM
* @param cover Que tan poblado est el cielo
* @param sharpness Que tan finas son las nubes
* @param num_octavos Numero de octavos usados en Perlin
* @param width Ancho de la textura
* @param height Alto de la textura
*/
Clouds::Clouds(float cover, float sharpness, int num_octavos, int width, int height) : cover_(cover),
                                                                                       sharpness_(sharpness),
                                                                                       text_width_(width),
                                                                                       text_height_(height) {
    num_octaves_ = static_cast<unsigned char>(num_octavos);
    m_texture = -1;
}


/**
*@brief Destructor de la clase, libera los vertices alojados
*la textura sin alpha, la textura con alpha (tanto la generada por openGL como el arreglo), 
*los octavos y los indices
*/
Clouds::~Clouds() {
    delete[] map_;
    delete[] RGBA_text_;

    if (m_texture != -1) { glDeleteTextures(1, &m_texture); }

}


/**
 * @brief Regresa el ancho de la textura
 * @author Fernando Montes de Oca Cespedes
 * @date Saturday, August 11, 2007 9:36:56 PM
 * @retval m_text_width
 */
int Clouds::texture_width() {
    return text_width_;
}


/**
 * @brief Regresa el alto de la textura
 * @author Fernando Montes de Oca Cespedes
 * @date Saturday, August 11, 2007 9:38:18 PM
 * @retval m_text_height
 */
int Clouds::texture_height() {
    return text_height_;
}


/**
* @brief Regresa el identificador de la textura generada, -1 en caso contrario
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 10:09:30 AM
* @retval unsigned int Identificador de la textura generada
*/
unsigned int Clouds::texture_id() {
    return m_texture;
}

/**
* @brief Calcula el Alpha de una textura de 32 bits
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 5:01:40 PM
*/
void Clouds::calculaAlpha() {
    float color;
    int desplazamiento = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < text_width_; i++) {
        for (j = 0; j < text_height_; j++) {
            color = map_[i * text_width_ + j];
            desplazamiento = i * (text_height_ * 4) + (j * 4);
            RGBA_text_[desplazamiento] = static_cast<char>(color);
            RGBA_text_[desplazamiento + 1] = static_cast<char>(color);
            RGBA_text_[desplazamiento + 2] = static_cast<char>(color);
            if (color == 0.0F) {
                RGBA_text_[desplazamiento + 3] = 0;
            } else {
                RGBA_text_[desplazamiento + 3] = static_cast<char>(255);
            }
        }
    }
}

/**
 * @brief Genera la textura con el alto y el ancho especificado
 * agrega canal alpha al mapa de textura y destruye la textura anterior
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 9:40:02 PM
 */
void Clouds::generaTexturaGL() {

    if (map_ != nullptr && RGBA_text_ != nullptr) {
        calculaAlpha();

        glDeleteTextures(1, &m_texture);
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, text_width_, text_height_, GL_RGBA, GL_UNSIGNED_BYTE, RGBA_text_);
    }
}

/**
* @brief Regresa una referencia constante a m_RGBA_text
* @author Fernando Montes de Oca Cespedes
* @date Saturday, October 20, 2007 7:06:58 PM
* @retval Apuntador a datos constantes de la imagen  
*/
char *Clouds::getData() {
    return RGBA_text_;
}