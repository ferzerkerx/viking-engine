/**
* @file Model3D.h
* @brief Encabezado de la clase Model3D
* @date Saturday, November 03, 2007 11:14:28 AM
*/
#ifndef __MODEL3D_H__
#define __MODEL3D_H__

#include <vector>
#include "../math/vector.h"
#include "Model3DLoader.h"

struct Face{
	int vert_index[3];			//indices de los vertices que conforman la cara
	int st_index[3];			//indices de las coordenadas de textura de la cara
};

struct Animacion {
	char nombre[255];			//nombre de la animacion
	int frame_inicial;			//frame inicial de la animacion
	int frame_final;			//frame final de la animacion
};


struct Material{
	char  nombre[255];			//nombre del materia
	color3f	color;				//color del material
	int   texture_id;			// identificador de la textura de openGL
	float u_tile;				// u tiling de la textura  
	float v_tile;				// v tiling de la textura  
	float u_offset;			    // u offset de la textura  
	float v_offset;				// v offset de la textura  
} ;


struct Object3D {
	int  num_verts;			//numero de vertices del objeto
	int  num_faces;			//numero de caras
	int  num_st;			//numero de coordenadas para texturizacion 
	int  num_materiales;	//numero de materiales del objeto
	int  * materials_ids;		//identificadores de los materiales del objeto
    int  * material_vert_index;	//indices que indican cuando se cambia un material
	char nombre[255];			//nombre del objeto
	vector3f  *vertices;		//vertices del objeto
	vector3f  *normales;		//normales de los vertices del objeto
	vector2f  *text_st;			//coordenadas de texturizacion
	Face *faces;				//caras del objeto
	Animacion *anims;
};


class Model3DLoader;

/**
* @class Model3D
* @author Fernando Montes de Oca Cspedes
* @date Friday, November 23, 2007 6:16:49 PM
* @brief Esta clase representa un modelo 3D con materiales
* y objetos
*/ 
class Model3D {

	protected:
		std::vector<Material> m_materials;	//Los materiales del modelo
		std::vector<Object3D> m_objects;	//Los objetos que representan el modelo
        Model3DLoader * m_ml;				//Un apuntador a un Loader que lea el modelo 3D

	public:
		~Model3D();
		void addObject(Object3D obj);
		void addMaterial(Material mat);
		virtual void render()=0;
		virtual void actualiza()=0;

};

/**
* @var Model3D::m_materials
* @brief Los materiales del modelo
*/

/**
* @var Model3D::m_objects
* @brief Los objetos que representan el modelo
*/

/**
* @var Model3D::m_ml
* @brief Un apuntador a un Loader que lea el modelo 3D
*/



#endif