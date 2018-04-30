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
    m_cloud_cover = 45.0f;
    m_ca = new CloudsActualizables();
    m_sp = new TexturedSkyPlane(16, 50.0f, 90.0f, 1.0f, 1.0f, m_ca->getTextureId());
}

VKSkyDome::VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura) : TexturedSkyDome(dphi, dtheta, r,
                                                                                                   id_textura) {
    FN("VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura)");
    LOG(0, "VKSkyDome creado %f %f %f ", dphi, dtheta, r);
    m_cloud_cover = 45.0f;
    m_ca = new CloudsActualizables();
    m_sp = new TexturedSkyPlane(16, 50.0f, 90.0f, 1.0f, 1.0f, m_ca->getTextureId());
}

VKSkyDome::~VKSkyDome() {
    delete m_ca;
    delete m_sp;

    LOG(0, "VKSkyDome destruido");
}


void VKSkyDome::render(vector3f poscam) {
    TexturedSkyDome::render(poscam);
}

void VKSkyDome::actualiza(vector3f poscam) {
    render(poscam);
    m_sp->actualiza(poscam);
}


void VKSkyDome::escribeImagenDeNubes() {
    auto *bmpwr = new BMPWriter("test.bmp", m_ca->getTextureWidth(), m_ca->getTextureHeight(), 4, m_ca->getData());
    bmpwr->writeTexture();

    delete bmpwr;
}