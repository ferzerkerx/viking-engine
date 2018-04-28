/**
* @file SkyBox.h
* @brief Encabezado de la clase SkyBox
* @date Saturday, October 20, 2007 1:28:01 PM
*/
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Sky.h"

/**
* @class SkyBox
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:28:06 PM
* @brief Esta clase dibuja un cubo texturizado
* utilizando GL_CLAMP para que no se aprecien los bordes
*/ 

class SkyBox : public Sky{
	protected:
		void render(vector3f poscam);
		unsigned int m_caras [6];	//almacena los identificadores de textura de la caja
		float m_alto;	//altura/2 de la caja (eje y openGL)
		float m_largo;	//largo/2 de la caja (eje x openGL)
		float m_ancho;	//ancho/2 de la caja (eje z openGL)

		enum Faces{FRONT,BACK,LEFT,RIGHT,UP,DOWN}; // caras de una caja
		
	public:
		SkyBox(float alto, float largo, float ancho, unsigned int* caras);
		SkyBox(float alto, float largo, float ancho,  unsigned int front, 
			unsigned int back, unsigned int left, unsigned int right, unsigned int up, unsigned int down);
		~SkyBox();
		void actualiza(vector3f poscam);
		float getAlto();
		float getLargo();
		float getAncho();

};

/**
* @var SkyBox::m_caras
* @brief almacena los identificadores de textura de la caja
*/

/**
* @var SkyBox::m_alto
* @brief altura/2 de la caja (eje y openGL)
*/

/**
* @var SkyBox::m_largo
* @brief largo/2 de la caja (eje x openGL)
*/

/**
* @var SkyBox::m_ancho
* @brief ancho/2 de la caja (eje z openGL)
*/



#endif