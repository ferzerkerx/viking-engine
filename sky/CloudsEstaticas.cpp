/**
* @file CloudsEstaticas.cpp
* @brief Implementacin de la clase CloudsEstaticas
* @date Saturday, August 11, 2007 9:54:57 PM
*/
#include <GL/glut.h>
#include <stdio.h>
#include "CloudsEstaticas.h"


/**
*@copydoc Clouds::Clouds()
*/
CloudsEstaticas::CloudsEstaticas() : Clouds(){
	generaTextura();
}

/**
*@copydoc Clouds::Clouds(float cover, float sharpness, int num_octavos, int width, int height)
*/
CloudsEstaticas::CloudsEstaticas(float cover, float sharpness, int num_octavos, int width, int height) : Clouds(cover, sharpness, num_octavos, width, height){
	generaTextura();
}

/**
*@brief Destructor de la clase, libera los vertices alojados
*/
CloudsEstaticas::~CloudsEstaticas(){
	
}


/**
 * @copydoc Clouds::generaTexturaGL()	
 * @brief Inicializa y destruye las estructuras necesarias para calcular
 * la textura
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 9:40:02 PM
 */
void CloudsEstaticas::generaTextura(){
	
	m_map = new float[m_text_width*m_text_height];
	m_RGBA_text = new char [m_text_width*m_text_height*4];

	Perlin::setNoise(m_map32);
	Perlin::overlapOctaves(m_map32, m_map,m_text_width, m_text_height);
	Perlin::expFilter(m_map,m_text_width,m_text_height,m_cover,m_sharpness);

	Clouds::generaTexturaGL();

	if (m_map != NULL){delete [] m_map; m_map = NULL;}
	if (m_RGBA_text != NULL){ delete [] m_RGBA_text; m_RGBA_text = NULL;}
}


/**
 * @brief Fuerza una actualizacin completa de la textura
 * @warning Bastante lenta puede generar un brinco mientras se juega
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:01:23 PM
 */
void CloudsEstaticas::forceUpdate(){
	generaTextura();
}