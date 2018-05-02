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
    VKSkyDome(float r, unsigned int id_texture);

    VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura);

    ~VKSkyDome() override;

    void Update(vector3f camera_position) override;

    void WriteCloudImage();

private:
    UpdatableClouds *updatable_clouds_;
    TexturedSkyPlane *textured_sky_plane_;
    float cloud_cover_;
};


#endif