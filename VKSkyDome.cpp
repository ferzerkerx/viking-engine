/**
* @file VKSkyDome.cpp
* @brief Implementacin de la clase VKSkyDome
* @date Friday, October 19, 2007 5:18:30 PM
*/

#include "VKSkyDome.h"
#include "loggers\EventLogger.h"

VKSkyDome::VKSkyDome(float r, unsigned int id_textura) : TexturedSkyDome(r, id_textura){
	LOG(0,"VKSkyDome(float r, unsigned int id_textura)");
	m_cloud_cover = 45.0f;
	m_ca = new CloudsActualizables();
	m_sp = new TexturedSkyPlane(16,50.0f,90.0f,1.0f,1.0f,m_ca->getTextureId());

	//m_c->setCover(m_cloud_cover);
}

VKSkyDome::VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura): TexturedSkyDome(dphi,dtheta,r,id_textura){
	FN("VKSkyDome(float dphi, float dtheta, float r, unsigned int id_textura)");
	LOG(0,"VKSkyDome creado %f %f %f ", dphi, dtheta, r);
	m_cloud_cover = 45.0f;
	m_ca = new CloudsActualizables();
	m_sp = new TexturedSkyPlane(16,50.0f,90.0f,1.0f,1.0f,m_ca->getTextureId());
	//m_c->setCover(m_cloud_cover);
	//m_c->setWindFactorX(0.002);
	//m_c->setWindFactorY(0.0005);
	//m_c->setTextureResolution(512,512);
}

VKSkyDome::~VKSkyDome(){
	if (m_ca != NULL){delete m_ca; m_ca = NULL;}
	if (m_sp != NULL){delete m_sp; m_sp = NULL;}
	LOG(0,"VKSkyDome destruido");
}

/**
 * @brief regresa la cantidad de cloud cover que hay
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 18, 2007 12:13:57 PM
 * @retval la cantidad de cloud cover que hay
 */
float VKSkyDome::getCloudCover(){
	return m_cloud_cover;
}

void VKSkyDome::incrementaCloudCover(){
	FN("incrementaCloudCover()");
	m_cloud_cover += 1.0f;
	m_ca->setCover(m_cloud_cover);
}

void VKSkyDome::decrementaCloudCover(){
	m_cloud_cover -= 1.0f;
	m_ca->setCover(m_cloud_cover);
}

void VKSkyDome::render(vector3f poscam){
	TexturedSkyDome::render(poscam);
}

void VKSkyDome::actualiza(vector3f poscam){
	render(poscam);
	m_ca->actualizaPorCoordenada();
	m_sp->actualiza(poscam);
}

#include "BMPWriter.h"
void VKSkyDome::escribeImagenDeNubes(){
	BMPWriter *bmpwr = new BMPWriter("test.bmp",m_ca->getTextureWidth(), m_ca->getTextureHeight(), 4,m_ca->getData());
	bmpwr->writeTexture();

	delete bmpwr;
}