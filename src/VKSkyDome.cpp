#include "VKSkyDome.h"
#include "framework/Framework.h"
#include "framework/BMPWriter.h"

VKSkyDome::VKSkyDome(float r, unsigned int id_texture) : TexturedSkyDome(r, id_texture) {
    LOG(0, "VKSkyDome(float r, unsigned int id_texture)");
    cloud_cover_ = 45.0F;
    updatable_clouds_ = new UpdatableClouds();
    textured_sky_plane_ = new TexturedSkyPlane(16, 50.0F, 90.0F, 1.0F, 1.0F, updatable_clouds_->texture_id());
}

VKSkyDome::VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura) : TexturedSkyDome(dphi, dtheta, r,
                                                                                                   id_textura) {
    FN("VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura)");
    LOG(0, "VKSkyDome creado %f %f %f ", dphi, dtheta, r);
    cloud_cover_ = 45.0F;
    updatable_clouds_ = new UpdatableClouds();
    textured_sky_plane_ = new TexturedSkyPlane(16, 50.0F, 90.0F, 1.0F, 1.0F, updatable_clouds_->texture_id());
}

VKSkyDome::~VKSkyDome() {
    delete updatable_clouds_;
    delete textured_sky_plane_;

    LOG(0, "VKSkyDome destruido");
}


void VKSkyDome::Render(vector3f camera_position) {
    TexturedSkyDome::Render(camera_position);
}

void VKSkyDome::Update(vector3f poscam) {
    Render(poscam);
    textured_sky_plane_->Update(poscam);
}


void VKSkyDome::WriteCloudImage() {
    auto *bmpwr = new BMPWriter("test.bmp", updatable_clouds_->texture_width(), updatable_clouds_->texture_height(), 4, updatable_clouds_->getData());
    bmpwr->WriteTexture();

    delete bmpwr;
}