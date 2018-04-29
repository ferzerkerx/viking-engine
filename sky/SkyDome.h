/**
* @file SkyDome.h
* @brief Encabezado de la clase SkyDome
* @date Friday, October 19, 2007 9:07:11 PM
*/
#ifndef	__SKYDOME__H__
#define __SKYDOME__H__

#include <math.h>
#include "Sky.h"


/**
*PI Es el valor de PI
*@todo Tal vez ponerlo en un archivo donde esten todas las constantes
*/
#define PI 3.1416f

/**
*RAD Es el valor de un radian
*@todo Tal vez ponerlo en un archivo donde esten todas las constantes
*/
#define RAD PI/180.0f

/**	@class SkyDome
*	@author Fernando Montes de Oca Cspedes.
*	@version 1.0
*	@date	09/May/2007
*	@brief Esta clase genera la mitad de una esfera que va a ser 
*	el domo del juego actualiza la posicion del sol
*/
class SkyDome : public Sky{

	protected:
		float m_dphi;			//define el delta de phi (0-90) usado para las divisiones
        float m_dtheta;			//define el delta de theta (0-360) usado para las divisiones
	    float m_r;				//radio de la hemiesfera
		int m_num_vert;			//guarda el numero de vertices
		vector3f m_sun_vector;	//representa la posicion x,y,z del sol
		polar3f m_sun_polar;	//representa la posicion del sol en coordenadas polares
		unsigned int m_sun_text; //identificador de la textura del sol
		bool m_has_sun_text;	//indica si tiene textura de sol
		
		/**
		*@struct vertxDome
		*@brief Contiene datos necesarios para cada vertice del domo
		*/
		typedef struct{
			polar3f pol;		//coordenada polar del vertice
			vector3f pos;		//posicion del vertice
			vector3f uv;		//usados para texturizar el domo
			color3f color;
		}vertxDome;

		vertxDome * m_vrtx;		//guarda los vertices de nuestro domo

		void generaHemiEsfera();
		virtual void renderDome(vector3f poscam) = 0;
		void renderSun(vector3f poscam);

	public:
		SkyDome(float r);
		SkyDome(float dphi, float dtheta, float r);
		~SkyDome();
		 void setSunTexture(unsigned int id);		
};

/**
* @var SkyDome::m_dphi
* @brief define el delta de phi (0-90) usado para las divisiones
*/

/**
* @var SkyDome::m_dtheta
* @brief define el delta de theta (0-360) usado para las divisiones
*/

/**
* @var SkyDome::m_r
* @brief radio de la hemiesfera
*/

/**
* @var SkyDome::m_num_vert
* @brief guarda el numero de vertices
*/

/**
* @var SkyDome::m_sun_vector
* @brief representa la posicion x,y,z del sol
*/

/**
* @var SkyDome::m_sun_polar
* @brief representa la posicion del sol en coordenadas polares
*/

/**
* @var SkyDome::m_sun_text
* @brief identificador de la textura del sol
*/


/**
* @var SkyDome::vertxDome::pol
* @brief coordenada polar del vertice
*/

/**
* @var SkyDome::vertxDome::pos
* @brief posicion del vertice
*/

/**
* @var SkyDome::vertxDome::uv
* @brief usados para texturizar el domo
*/

/**
* @var SkyDome::m_vrtx
* @brief guarda los vertices de nuestro domo
*/


#endif