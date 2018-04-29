/**
* @file VKSkyDome.h
* @brief Encabezado de la clase VKSkyDome
* @date Friday, October 19, 2007 5:18:25 PM
*/
#ifndef __VKSKYDOME_H__
#define __VKSKYDOME_H__

#include "sky/TexturedSkyDome.h"
#include "sky/TexturedSkyPlane.h"
#include "sky/CloudsActualizables.h"


/**
* @class VKSkyDome
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 12:16:55 PM
* @brief Domo del juego  
*/

class VKSkyDome : public TexturedSkyDome {
public:
    VKSkyDome(float r, unsigned int id_textura);

    VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura);

    ~VKSkyDome();

    float getCloudCover();

    void incrementaCloudCover();

    void decrementaCloudCover();

    void actualiza(vector3f poscam);

    void escribeImagenDeNubes();

private:
    CloudsActualizables *m_ca;    //apuntador a la instancia de Clouds
    TexturedSkyPlane *m_sp;                //apuntador a la instancia de TexturedSkyPlane
    float m_cloud_cover;        //guarda el cover de las nubes
    void render(vector3f poscam);
};

/**
* @var VKSkyDome::m_ca
* @brief apuntador a la instancia de Clouds
*/

/**
* @var VKSkyDome::m_sp
* @brief apuntador a la instancia de TexturedSkyPlane
*/

/**
* @var VKSkyDome::m_cloud_cover
* @brief guarda el cover de las nubes
*/


#endif