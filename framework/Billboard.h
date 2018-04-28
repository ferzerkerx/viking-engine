
#ifndef _BILLBOARD_H
#define _BILLBOARD_H

/**
*@file Billboard.h
*@brief Encabezado de la clase Billboard
*/

#include "math/vector.h"

/**	@class Billboard
*	@author Luis Daniel Rodrguez P.
*	@version 2.0
*	@date	09/Jun/2007
*	@brief Esta clase permite la opcin de billboard en objetos. Se tiene billboard
*	real y falso.
*	@todo Corregir algunos bugs de rotacin.
*	NOTA: El billboard real permite otras aplicaciones como hacer un 'face' entre 2 objetos.
*/



class Billboard {
							
	public:
		static void BBFalsoEsfera();
		static void BBFalsoCilindro();

		static void BBCilindro(vector3f camP, float objPosX, float objPosY, float objPosZ);
		static void BBEsfera(vector3f camP, float objPosX, float objPosY, float objPosZ);

		static void BBFin();		
};

#endif
