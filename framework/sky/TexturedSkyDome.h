/**
* @file TexturedSkyDome.h
* @brief Encabezado de la clase TexturedSkyDome
* @date Wednesday, August 15, 2007 2:56:06 PM
*/
#ifndef __TEXTUREDSKYDOME_H__
#define __TEXTUREDSKYDOME_H__


#include "SkyDome.h"

/**	@class TexturedSkyDome
*	@author Fernando Montes de Oca Cspedes.
*	@version 1.0
*	@date	09/May/2007
*	@brief Esta clase es un SkyDome texturizado
*/
class TexturedSkyDome : public SkyDome {
	protected:

		unsigned int m_textura; //guarda el identificador de la textura a ser usada en el domo
		float m_h_tile;	//Tile horizontal
		float m_v_tile;	//Tile vertical
		void render(vector3f poscam);
		void renderDome(vector3f poscam);
        void calculaUV();

		
	public:
		TexturedSkyDome(float r, unsigned int id_textura);
		TexturedSkyDome(float dphi, float dtheta, float r, unsigned int id_textura);
		~TexturedSkyDome();
		void actualiza(vector3f poscam);
		void setTexture(unsigned int id);
      			
};

/**
* @var TexturedSkyDome::m_textura
* @brief guarda el identificador de la textura a ser usada en el domo
*/

/**
* @var TexturedSkyDome::m_h_tile
* @brief Tile horizontal
*/

/**
* @var TexturedSkyDome::m_v_tile
* @brief Tile vertical
*/






#endif

