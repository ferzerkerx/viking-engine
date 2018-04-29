#include "definiciones.h"
#include "Billboard.h"

/**
*@brief Genera un billboarding no real, el objeto no enfrenta la camara sino un plano perpendicular a la
*camara. Es el ms fcil de implementar. Es billboard de tipo esfrico o de punto.
 */
void Billboard::BBFalsoEsfera() {
	float modelview[16];
	int i,j;
	
	glPushMatrix();	// Guardamos la matriz modelview actual
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview); // Obtenemos la matriz modelview actual
	// Nos movemos dentro de la matriz modelview  
	for(i=0; i<3; i++) 
		for(j=0; j<3; j++) {
			if(i==j)
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
	glLoadMatrixf(modelview); // Asignamos la matriz modelview 
}

/**
*@brief Genera billboarding real de tipo esfrico o punto, se puede enfrentar un objeto a cualquier posicin u objeto.
*@param camP Vector de posicin de la cmara.
*@param objPosX Posicin en X del objeto que tendr el efecto de billboarding.
*@param objPosY Posicin en Y del objeto que tendr el efecto de billboarding.
*@param objPosZ Posicin en Z del objeto que tendr el efecto de billboarding.
*/
void Billboard::BBEsfera(vector3f camP, float objPosX, float objPosY, float objPosZ) {

    vector3f lookAt, objACamProj, objACam, upAux;
	float cosAngulo;

	glPushMatrix();
	objACamProj.x = camP.x - objPosX ;
	objACamProj.y = 0;
	objACamProj.z = camP.z - objPosZ ;

	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 1;

	objACamProj = Normalizar(objACamProj);

	upAux = Cruzado(lookAt,objACamProj);

	cosAngulo = Punto(lookAt, objACamProj);

    if((cosAngulo < 0.9999) && (cosAngulo > -0.9999))
		glRotatef((float)(acos(cosAngulo)*180/3.1416),upAux.x, upAux.y, upAux.z);	
      
	objACam.x = camP.x - objPosX;
	objACam.y = camP.y - objPosY;
	objACam.z = camP.z - objPosZ;

	objACam = Normalizar(objACam);

	cosAngulo = Punto(objACamProj, objACam);

	if((cosAngulo < 0.9999) && (cosAngulo > -0.9999))
		if (objACam.y < 0)
			glRotatef((float)(acos(cosAngulo)*180/3.1416),1,0,0);	
		else
			glRotatef((float)(acos(cosAngulo)*180/3.1416),-1,0,0);	
}

/**
*@brief Genera un billboarding no real, el objeto no enfrenta la camara sino un plano perpendicular a 
*la camara, version cilindro, el 'up vector' no cambia.
*/
void Billboard::BBFalsoCilindro() {
	float modelview[16];
	int i,j;
	
	glPushMatrix(); // Guardamos la matriz modelview actual
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview); // Obtenemos la matriz modelview actual
	// Nos movemos dentro de la matriz modelview
	for(i=0; i<3; i+=2) // Nos saltamos la columna del up vector (2da.) 
		for(j=0; j<3; j++) {
			if(i==j)
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
	glLoadMatrixf(modelview); // Asignamos la matriz modelview 
}

/**
*@brief Genera billboarding real, se puede enfrentar un objeto a cualquier posicin u objeto, es te tipo esfrico (slo rota en el eje Y).
*@param camP Vector de posicin de la cmara.
*@param objPosX Posicin en X del objeto que tendr el efecto de billboarding.
*@param objPosY Posicin en Y del objeto que tendr el efecto de billboarding.
*@param objPosZ Posicin en Z del objeto que tendr el efecto de billboarding.
*/
void Billboard::BBCilindro(vector3f camP, float objPosX, float objPosY, float objPosZ) {
 
	float cosAngulo;
	vector3f objACamProj, lookAt, upAux;

	glPushMatrix();
	objACamProj.x = camP.x - objPosX ;
	objACamProj.y = 0;
	objACamProj.z = camP.z - objPosZ ;

	lookAt.x = 0;
	lookAt.y = 0;
	lookAt.z = 1;

	objACamProj = Normalizar(objACamProj);

	upAux = Cruzado(lookAt, objACamProj); 

	cosAngulo = Punto(lookAt, objACamProj);	

	if((cosAngulo < 0.9999) && (cosAngulo > -0.9999))
		glRotatef((float)(acos(cosAngulo)*180/3.1416),upAux.x, upAux.y, upAux.z);	
}

/**
*@brief Hace un pop de la matriz modelview para cerrar el billboarding.
*/
void Billboard::BBFin() {
	// Reasignar la matriz modelview antes guardada
	glPopMatrix();
}