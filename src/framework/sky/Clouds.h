#ifndef __CLOUDS__H__
#define __CLOUDS__H__


#include "Perlin.h"
#include "../Framework.h"


/**
* @class Clouds
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 5:14:06 PM
*/

class Clouds {
public:
    Clouds();

    Clouds(float cover, float sharpness, int num_octavos, int width, int height);

    int texture_width();

    int texture_height();

    unsigned int texture_id();

    char *getData();

protected:
    virtual ~Clouds();

    unsigned char num_octaves_;    //define la cantidad de octavos de perlin que se desea
    float sharpness_;                //define que tan delgadas son las nubes
    float cover_;                    //define que tan nuboso debe estar el cielo
    char *RGBA_text_;                //usado para generar la textura con alpha
    float map32_[32 * 32];            //mapa de ruido
    float *map_;                    //mapa que contiene la textura sin alpha
    int text_width_;                //ancho de la textura deseada
    int text_height_;                //alto de la textura deseado
    unsigned int m_texture;            //identificador de la textura generada por openGL

    virtual void CreateTexture()= 0;

    virtual void ForceUpdate()=0;

    void calculaAlpha();

    void generaTexturaGL();
};

#endif