/**
* @file Model3D.cpp
* @brief Implementacin de la clase Model3D
* @date Saturday, November 03, 2007 11:14:34 AM
*/

#include "Model3D.h"

/**
* @brief Destructor, libera todos los recursos alojados
* para cada objeto del modelo
* @author Fernando Montes de Oca Cspedes
* @date Friday, November 23, 2007 7:07:46 PM
*/
Model3D::~Model3D(){
	int i = 0;
	while (m_objects.size() > 0){
		i= m_objects.size()-1;
		if (m_objects[i].materials_ids)	 {delete m_objects[i].materials_ids;}
		if (m_objects[i].material_vert_index){delete m_objects[i].material_vert_index;}
		if (m_objects[i].vertices)			 {delete m_objects[i].vertices;}
		if (m_objects[i].normales)		  {delete m_objects[i].normales;}
		if (m_objects[i].text_st)		  {delete m_objects[i].text_st;}
		if (m_objects[i].faces)			  {delete m_objects[i].faces;}
		m_objects.pop_back();
	}

	while (m_materials.size() > 0){
		m_materials.pop_back();
	}
}

/**
* @brief Mete un objeto a nuestro vector de objetos
* @author Fernando Montes de Oca Cspedes
* @date Friday, November 23, 2007 7:09:07 PM
* @param obj El objeto a insertar en el modelo
*/
void Model3D::addObject(Object3D obj){
	m_objects.push_back(obj);
}

/**
* @brief Mete un nuevo material al modelo
* @author Fernando Montes de Oca Cspedes
* @date Friday, November 23, 2007 7:09:44 PM
* @param mat Material a insertar en el modelo
*/
void Model3D::addMaterial(Material mat){
	m_materials.push_back(mat);
}