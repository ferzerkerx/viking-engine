/**
* @file TexturedSkyPlane.h
* @brief Encabezado de la clase TexturedSkyPlane
* @date Thursday, October 18, 2007 7:23:33 PM
*/

#ifndef __TEXTUREDSKYPLANE__H__
#define __TEXTUREDSKYPLANE__H__

#include "Sky.h"


/**
*SQR Eleva al cuadrado cualquier numero
*@todo Tal vez ponerlo en un archivo donde esten todas las constantes
*/
#define SQR(x)(x*x)


/**
* @class TexturedSkyPlane
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 9:36:24 AM
* @brief Es un TexturedSkyPlane texturizado, es similar
* a un TexturedSkyDome con la diferencia que se usa un plano torcido
* en lugar de una mitad de esfera
*/
class TexturedSkyPlane : public Sky {
private:
    /**
    *@struct vertx
    *@brief Contiene datos necesarios de cada vertice del Skyplane
    */
    typedef struct {
        vector3f pos;                //posicion x_,y_,z_ del vertice
        float u;
        float v;                    //usados para texturizacin
    } vertx;

    float m_h_tile;                    //horizontal tile para texturizacion
    float m_v_tile;                    //vertical tile para texturizacion
    unsigned int m_texture;            //textura del TexturedSkyPlane
    unsigned int num_vertex_;    //guarda el nmero de vertex
    unsigned int num_indexes_;        //guarda el nmero de indices
    unsigned int divisions_;        //contiene el nmero de divisiones
    float atmosphere_radius_;        //es el radio atmosfrico del TexturedSkyPlane
    unsigned int *indexes_;        //guarda todos los indices (indica como deben ser dibujados los vertex)
    vertx *vertex_;            //guarda todos los vertex
    float planet_radius_;            //radio planetario del TexturedSkyPlane
    float roty_factor_;            //factor de rotacin en y_(delta)
    float m_wind_factor_x;            //define el movimiento de la textura en x_;
    float m_wind_factor_y;            //define el movimiento de la textura en y_;
    unsigned int m_update_text_milli;//define cada cuando se debe actualizar la textura
    unsigned int rot_texture_in_milli_; //define cada cuando se debe rotar
    float texture_delta_x;            //define la rotacin de la textura en x_
    float m_desface_text_y;            //define la rotacin de la textura en y_
    Timer timer_;                //Timer que controla el movimiento de las nubes

    void Render(vector3f poscam) override;

    void GenerateSkyPlane();


public:
    TexturedSkyPlane(int divisions, float planet_radius, float atmosphere_radius, float h_tile, float v_tile,
                     unsigned int texture);

    void Update(vector3f poscam) override;

    void setWindFactorX(float wfx);

    void setWindFactorY(float wfy);

    void setWindFactorXY(float wfx, float wfy);

    ~TexturedSkyPlane() override;

};

/**
* @var TexturedSkyPlane::vertx::pos
* @brief posicion x,y,z del vertice
*/

/**
* @var TexturedSkyPlane::vertx::u,v
* @brief usados para texturizacin
*/

/**
* @var TexturedSkyPlane::m_h_tile
* @brief horizontal tile para texturizacion
*/

/**
* @var TexturedSkyPlane::m_v_tile
* @brief vertical tile para texturizacion
*/

/**
* @var TexturedSkyPlane::m_texture
* @brief textura del TexturedSkyPlane
*/

/**
* @var TexturedSkyPlane::m_num_vertices
* @brief guarda el nmero de vertices
*/

/**
* @var TexturedSkyPlane::m_num_indices
* @brief guarda el nmero de indices
*/

/**
* @var TexturedSkyPlane::m_divisiones
* @brief contiene el nmero de divisiones
*/

/**
* @var TexturedSkyPlane::m_atmosphere_radius
* @brief es el radio atmosfrico del TexturedSkyPlane
*/

/**
* @var TexturedSkyPlane::m_indices
* @brief guarda todos los indices (indica como deben ser dibujados los vertices)
*/

/**
* @var TexturedSkyPlane::m_vertices
* @brief guarda todos los vertices
*/

/**
* @var TexturedSkyPlane::m_planet_radius
* @brief radio planetario del TexturedSkyPlane
*/

/**
* @var TexturedSkyPlane::m_roty_factor
* @brief factor de rotacin en y(delta)
*/

/**
* @var TexturedSkyPlane::m_wind_factor_x
* @brief define el movimiento de la textura en x;
*/

/**
* @var TexturedSkyPlane::m_wind_factor_y
* @brief define el movimiento de la textura en y;
*/

/**
* @var TexturedSkyPlane::m_update_text_milli
* @brief define cada cuando se debe actualizar la textura
*/

/**
* @var TexturedSkyPlane::m_rot_text_milli
* @brief define cada cuando se debe rotar
*/

/**
* @var TexturedSkyPlane::m_desface_text_x
* @brief define la rotacin de la textura en x
*/

/**
* @var TexturedSkyPlane::m_desface_text_y
* @brief define la rotacin de la textura en y
*/

/**
* @var TexturedSkyPlane::m_mov_timer
* @brief Timer que controla el movimiento de las nubes
*/


#endif