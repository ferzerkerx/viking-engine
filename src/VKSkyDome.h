#ifndef __VKSKYDOME_H__
#define __VKSKYDOME_H__

#include "framework/sky/TexturedSkyDome.h"
#include "framework/sky/TexturedSkyPlane.h"
#include "framework/sky/UpdatableClouds.h"


/**
* @class VKSkyDome
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 12:16:55 PM
*/

class VKSkyDome : public TexturedSkyDome {
public:
    VKSkyDome(float r, unsigned int id_textura);

    VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura);

    ~VKSkyDome();

    void actualiza(vector3f poscam);

    void escribeImagenDeNubes();

private:
    UpdatableClouds *m_ca;    //apuntador a la instancia de Clouds
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