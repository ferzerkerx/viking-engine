/**
* @file Clouds.h
* @brief Encabezado de la clase Clouds
* @date Friday, October 19, 2007 5:18:17 PM
*/
#ifndef __CLOUDS__H__
#define __CLOUDS__H__


#include "Perlin.h"
#include "../Framework.h"


/**
* @class Clouds
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 5:14:06 PM
* @brief Define las estructuras bsicas para la generacion de texturas
* es una interfaz para generadores de texturas de nubes ms especficos
*/

class Clouds {
public:
    Clouds();

    Clouds(float cover, float sharpness, int num_octavos, int width, int height);

    ~Clouds();

    int getTextureWidth();

    int getTextureHeight();

    unsigned int getTextureId();

    char *getData();

protected:
    unsigned char m_num_octaves;    //define la cantidad de octavos de perlin que se desea
    float m_sharpness;                //define que tan delgadas son las nubes
    float m_cover;                    //define que tan nuboso debe estar el cielo
    char *m_RGBA_text;                //usado para generar la textura con alpha
    float m_map32[32 * 32];            //mapa de ruido
    float *m_map;                    //mapa que contiene la textura sin alpha
    int m_text_width;                //ancho de la textura deseada
    int m_text_height;                //alto de la textura deseado
    unsigned int m_texture;            //identificador de la textura generada por openGL

    virtual void generaTextura()= 0;

    virtual void forceUpdate()=0;

    void calculaAlpha();

    void generaTexturaGL();
};

/**
* @var Clouds::m_num_octaves
* @brief define la cantidad de octavos de perlin que se desea
*/

/**
* @var Clouds::m_sharpness
* @brief define que tan delgadas son las nubes
*/

/**
* @var Clouds::m_cover
* @brief define que tan nuboso debe estar el cielo
*/

/**
* @var Clouds::m_RGBA_text
* @brief usado para generar la textura con alpha
*/

/**
* @var Clouds::m_map32
* @brief mapa de ruido
*/

/**
* @var Clouds::m_map
* @brief mapa que contiene la textura sin alpha
*/

/**
* @var Clouds::m_text_width
* @brief ancho de la textura deseada
*/

/**
* @var Clouds::m_text_height
* @brief alto de la textura deseado
*/

/**
* @var Clouds::m_texture
* @brief identificador de la textura generada por openGL
*/


#endif