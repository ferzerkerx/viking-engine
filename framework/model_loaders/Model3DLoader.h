/**
* @file Model3DLoader.h
* @brief Encabezado de la clase Model3DLoader
* @date Saturday, November 03, 2007 11:14:40 AM
*/
#ifndef __MODEL3DLOADER__H__
#define __MODEL3DLOADER__H__

#include "..\textura.h"
#include "Model3D.h"

class Model3D;

/**
* @class Model3DLoader
* @author Fernando Montes de Oca Cspedes
* @date Friday, November 23, 2007 5:49:22 PM
* @brief Esta clase representa una interfaz para los 
* loaders de modelos3D
*/ 
class Model3DLoader {
	protected:
		Model3D * m_mdl;

	public:
		Model3DLoader (Model3D * mdl){
			m_mdl = mdl;	
		}
		~Model3DLoader(){
			m_mdl = NULL;	
		}

        /**
        * @brief Funcion que se encarga de importar el modelo
        * @author Fernando Montes de Oca Cspedes
        * @date Friday, November 23, 2007 7:06:23 PM
        * @param modelo El path hacia el archivo que contiene el modelo
        */
		virtual void importar(char * modelo) = 0;	
};

/**
* @var Model3DLoader::m_mdl
* @brief Apuntador a un objeto Model3D que representa un modelo 3D
*/



#endif