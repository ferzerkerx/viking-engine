/**
* @file CloudsActualizables.h
* @brief Encabezado de la clase CloudsActualizables
* @date Friday, October 19, 2007 5:18:06 PM
*/
#ifndef __CLOUDS_ACT_H__
#define __CLOUDS_ACT_H__

#include "Clouds.h"
#include "../Timer.h"

/**
* @class CloudsActualizables
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 5:12:31 PM
* @brief Genera una textura de nubes y permite actualizarla
* a diferentes velocidades, slo actualiza el cover sharpness
* tamao y el numero de octavos
*/ 

class CloudsActualizables : public Clouds{
	
	public:
		CloudsActualizables();
		CloudsActualizables(float cover, float sharpness, int num_octavos, int width, int height,unsigned int up_text_milli);
		~CloudsActualizables();
		int actualizaPorOctavo();
		int actualizaPorCoordenada();
		void forceUpdate();
		void setTextureUpdateTime(int milli);
		void setNumOctaves(unsigned char noctaves);
		void setCover(float cover);
		void setSharpness(float sharpness);
		void setTextureResolution (int width, int height);

	private:
		Timer m_text_timer;		//Timer que ayuda a saber cada cuando debe actualizarse la textura
		float m_brightness;		//guarda el valor del ruido interpolado(solo en actualizacin x coordenada)
		unsigned int m_update_text_milli;//define cada cuando se debe actualizar la textura
		unsigned char m_actual_octave;	//define el octavo actual (usado para actualizacin eficiente)

		/**
		*@struct octave
		*@brief Contiene datos necesarios de cada octavo 
		*(usado para actualizacin eficiente por coordenada)
		*/
		struct octave{
			int x;						//lleva la ultima x por ser actualizada
			float scale;				//contiene la escala de cada octavo
		}*m_octaves;					//guarda todos los octavos y sus datos

		void generaTextura();

};

/**
* @var CloudsActualizables::m_text_timer
* @brief Timer que ayuda a saber cada cuando debe actualizarse la textura
*/

/**
* @var CloudsActualizables::m_brightness
* @brief guarda el valor del ruido interpolado(solo en actualizacin x coordenada)
*/

/**
* @var CloudsActualizables::m_update_text_milli
* @brief define cada cuando se debe actualizar la textura
*/

/**
* @var CloudsActualizables::m_actual_octave
* @brief define el octavo actual (usado para actualizacin eficiente)
*/


/**
* @var CloudsActualizables::m_octaves
* @brief guarda todos los octavos y sus datos
*/ 

/**
* @var CloudsActualizables::octave::x
* @brief lleva la ultima x por ser actualizada
*/

/**
* @var CloudsActualizables::octave::scale
* @brief contiene la escala de cada octavo
*/


#endif