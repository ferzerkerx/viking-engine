/**
* @file Clouds.cpp
* @brief Implementacin de la clase Clouds
* @date Friday, October 19, 2007 5:18:12 PM
*/
#include <GL/glut.h>
#include <stdio.h>
#include "Clouds.h"



/**
*@brief Constructor de las nubes, inicializa los factores para generar la
*textura de nubes(cobertura, delgadez, numero de octavos,tamao de la textura)
* por default
*/
Clouds::Clouds(){
	m_cover = 45.0f;
	m_sharpness =	0.987f;
	m_num_octaves = 4;
	m_texture = -1;
    	
	m_text_width = 384;
	m_text_height = 384;
	
}


/**
* @brief Constructor de las nubes, inicializa los factores para generar la
* textura de nubes con parametros especificos
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 5:07:08 PM
* @param cover Que tan poblado est el cielo
* @param sharpness Que tan finas son las nubes
* @param num_octavos Numero de octavos usados en Perlin
* @param width Ancho de la textura
* @param height Alto de la textura
*/
Clouds::Clouds(float cover, float sharpness, int num_octavos, int width, int height){
	m_cover = cover;
	m_sharpness =	sharpness;
	m_num_octaves = num_octavos;
	m_texture = -1;
    m_text_width = width;
	m_text_height = height;
	
}


/**
*@brief Destructor de la clase, libera los vertices alojados
*la textura sin alpha, la textura con alpha (tanto la generada por openGL como el arreglo), 
*los octavos y los indices
*/
Clouds::~Clouds(){
	if (m_map != NULL){delete [] m_map;}

	if (m_RGBA_text != NULL){ delete [] m_RGBA_text;}

	if (m_texture != -1) {glDeleteTextures(1,&m_texture);}

}


/**
 * @brief Regresa el ancho de la textura
 * @author Fernando Montes de Oca Cspedes
 * @date Saturday, August 11, 2007 9:36:56 PM
 * @retval m_text_width
 */
int Clouds::getTextureWidth(){
    return m_text_width;
}


/**
 * @brief Regresa el alto de la textura
 * @author Fernando Montes de Oca Cspedes
 * @date Saturday, August 11, 2007 9:38:18 PM
 * @retval m_text_height
 */
int Clouds::getTextureHeight(){
	return m_text_height;
}


/**
* @brief Regresa el identificador de la textura generada, -1 en caso contrario
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 10:09:30 AM
* @retval unsigned int Identificador de la textura generada
*/
unsigned int Clouds::getTextureId(){
	return m_texture;
}

/**
* @brief Calcula el Alpha de una textura de 32 bits
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 5:01:40 PM
*/
void Clouds::calculaAlpha(){
	float color = 0.0;
	int desplazamiento = 0 ;
	int i, j = 0;

	for(i=0; i<m_text_width; i++)   {     
		for(j=0; j<m_text_height; j++) {
			color = m_map[i*m_text_width+j]; 
			desplazamiento = i*(m_text_height*4)+(j*4);
			m_RGBA_text[desplazamiento] = color;
			m_RGBA_text[desplazamiento+1] = color;
			m_RGBA_text[desplazamiento+2] = color;
			if (color == 0.0f){
				m_RGBA_text[desplazamiento+3] = 0;
			}
			else{
				m_RGBA_text[desplazamiento+3] = 255;
			}
		}
	}
}

/**
 * @brief Genera la textura con el alto y el ancho especificado
 * agrega canal alpha al mapa de textura y destruye la textura anterior
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 9:40:02 PM
 */
void Clouds::generaTexturaGL(){
	
	if (m_map != NULL && m_RGBA_text != NULL){
		calculaAlpha();

		glDeleteTextures(1,&m_texture);
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_text_width, m_text_height, GL_RGBA, GL_UNSIGNED_BYTE, m_RGBA_text);	
	}
}

/**
* @brief Regresa una referencia constante a m_RGBA_text
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 7:06:58 PM
* @retval Apuntador a datos constantes de la imagen  
*/
char * Clouds::getData(){
	return m_RGBA_text;
}