/*!
\file Fuente.h
\brief Encabezado de la clase Fuente
*/

#ifndef __FUENTE__H__
#define __FUENTE__H__

/*! \class Fuente
 *  \brief Esta clase crea una fuente de texto a partir de una imagen
	 y puede imprimir texto en pantalla
 *  \author Fernando Montes de Oca Cspedes
 *  \version 2.0
 *  \date 2006
 */
class Fuente{
	
	private:
		/*!
		\var textura
		\brief apuntador al id de la textura que contiene la fuente
		*/
		unsigned int *textura;
		/*!
		\var base
		\brief identificador de la base de la lista
		*/
		unsigned int base;
		

	public:
		/*!
		\param text apuntador al id de la textura que contiene la fuente
		*/
		Fuente(unsigned int * text);
		/*!
		\brief Coloca el apuntador en NULL y eliminamos las listas generadas
        */
		~Fuente();
		/*!
		\fn glPrint(float , float , char *, int, int, int )
		\param x Posicion en x del texto
		\param y Posicion en y del texto
		\param string texto a imprimir
        \param set Especifica que conjunto de elementos de la textura fuente se van a utilizar
		\param ancho define el ancho que se debe tomar para cambiar a modo ortogonal
		\param alto define el alto que se debe tomar para cambiar a modo ortogonal
		\brief Imprime el String especificado en la posicion indicada usando un set de la fuente
		*/
        void glPrint(float , float , char *, int, int, int );


};

#endif