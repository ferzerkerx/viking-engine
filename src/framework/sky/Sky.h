/**
* @file Sky.h
* @brief Encabezado de la clase Sky
* @date Thursday, October 18, 2007 6:57:57 PM
*/

#ifndef    __SKY_H__
#define __SKY_H__

#include "../Framework.h"

/**
* @class Sky
* @author Fernando Montes de Oca Cespedes
* @date Thursday, October 18, 2007 6:58:13 PM
* @brief Clase abstracta que representa un cielo y funciona como
* una interfaz para la implementacin de clases especificas
*/

class Sky {
public:

    /**
    *@brief Esta funcin actualiza los elementos del objeto Sky
    *@param poscam Es el vector de orientacin de la camara (en este caso es FPS)
    */
    virtual void actualiza(vector3f poscam)=0;

    /**
    *@brief Renderea el objeto Sky
    *@param poscam Es el vector de orientacin de la camara (en este caso es FPS)
    */
    virtual void render(vector3f poscam)=0;

    virtual ~Sky() {};
};


#endif