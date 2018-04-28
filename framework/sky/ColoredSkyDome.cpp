/**
* @file ColoredSkyDome.cpp
* @brief Implementacin de la clase ColoredSkyDome
* @date Saturday, August 11, 2007 10:24:46 PM
*/

#include "ColoredSkyDome.h"
#include <gl\glaux.h>
#include <stdio.h>

/**
*@copydoc SkyDome::SkyDome(float r)
*@brief Colorea el domo por default en azul
*/
ColoredSkyDome::ColoredSkyDome(float r) : SkyDome(r){
	coloreaDomo();
}


/**
*@copydoc SkyDome::SkyDome(float dphi,float dtheta,float r)
*@brief Colorea el domo por default en azul
*/
ColoredSkyDome::ColoredSkyDome(float dphi,float dtheta,float r): SkyDome(dphi,dtheta,r){
	coloreaDomo();
}

/**
@brief Destructor de la clase
*/
ColoredSkyDome::~ColoredSkyDome(){
	
}

/**
* @brief Renderea el Domo coloreado
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 8:44:26 PM
* @param poscam vector de la camara
*/
void ColoredSkyDome::renderDome(vector3f poscam){

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	int i = 0;
	glPushMatrix();
		glTranslatef(poscam.x, poscam.y, poscam.z);
		glRotatef(-90.0f,1.0,0.0,0.0f);
		glBegin(GL_TRIANGLE_STRIP);
			for (i=0; i<m_num_vert;i++){
				glColor3f(m_vrtx[i].color.r,m_vrtx[i].color.g,m_vrtx[i].color.b);
				glVertex3f(m_vrtx[i].pos.x,m_vrtx[i].pos.y,m_vrtx[i].pos.z);
			}
		glEnd();
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	
	glColor3f(1.0f,1.0f,1.0f);	
}


/**
*@copydoc Sky::render(vector3f poscam)
*@brief Renderea El ColoredSkyDome
*/
void ColoredSkyDome::render(vector3f poscam){
	renderDome(poscam);
	renderSun(poscam);
}

/**
*@copydoc Sky::actualiza(vector3f poscam)
*@brief Actualiza los elementos del ColoredSkyDome
*/
void ColoredSkyDome::actualiza(vector3f poscam){
	render(poscam);
}

void ColoredSkyDome::coloreaDomo(){
	for(int i = 0; i<m_num_vert; i++){
		m_vrtx[i].color = color3f(0.0,0.35f,0.7f);
	}
}