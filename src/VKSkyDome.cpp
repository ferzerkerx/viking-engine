/**
* @file VKSkyDome.cpp
* @brief Implementacin de la clase VKSkyDome
* @date Friday, October 19, 2007 5:18:30 PM
*/

#include "VKSkyDome.h"
#include "framework/Framework.h"
#include "framework/BMPWriter.h"

VKSkyDome::VKSkyDome(float r, unsigned int id_textura) : TexturedSkyDome(r, id_textura) {
    LOG(0, "VKSkyDome(float r, unsigned int id_textura)");
    m_cloud_cover = 45.0F;
    m_ca = new UpdatableClouds();
    m_sp = new TexturedSkyPlane(16, 50.0F, 90.0F, 1.0F, 1.0F, m_ca->texture_id());
}

VKSkyDome::VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura) : TexturedSkyDome(dphi, dtheta, r,
                                                                                                   id_textura) {
    FN("VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura)");
    LOG(0, "VKSkyDome creado %f %f %f ", dphi, dtheta, r);
    m_cloud_cover = 45.0F;
    m_ca = new UpdatableClouds();
    m_sp = new TexturedSkyPlane(16, 50.0F, 90.0F, 1.0F, 1.0F, m_ca->texture_id());
}

VKSkyDome::~VKSkyDome() {
    delete m_ca;
    delete m_sp;

    LOG(0, "VKSkyDome destruido");
}


void VKSkyDome::Render(vector3f poscam) {
    TexturedSkyDome::Render(poscam);
}

void VKSkyDome::Update(vector3f poscam) {
    Render(poscam);
    m_sp->Update(poscam);
}


void VKSkyDome::escribeImagenDeNubes() {
    auto *bmpwr = new BMPWriter("test.bmp", m_ca->texture_width(), m_ca->texture_height(), 4, m_ca->getData());
    bmpwr->writeTexture();

    delete bmpwr;
}