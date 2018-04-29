/**
* @file ColoredSkyDome.h
* @brief Encabezado de la clase ColoredSkyDome
* @date Friday, October 19, 2007 9:06:34 PM
*/
#ifndef __COLOREDSKYDOME__H__
#define __COLOREDSKYDOME__H__

#include "SkyDome.h"

/**
* @class ColoredSkyDome
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 9:05:57 PM
* @brief Es una clase que representa un SkyDome
* coloreado, la funcion de coloreado es por default azul claro
*/ 

class ColoredSkyDome : public SkyDome{
	public: 
		ColoredSkyDome(float r);
		ColoredSkyDome(float dphi,float dtheta,float r);
		~ColoredSkyDome();
		void actualiza(vector3f poscam);

	protected:
		void render(vector3f poscam);
		void renderDome(vector3f poscam);
		virtual void coloreaDomo();
		
};

#endif