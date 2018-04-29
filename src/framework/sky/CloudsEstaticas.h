/**
*@file CloudsEstaticas.h
*@brief Encabezado de la clase CloudsEstaticas
*/

#ifndef __CLOUDSESTATICAS__H
#define __CLOUDSESTATICAS__H


#include "Clouds.h"


/**	@class CloudsEstaticas
*	@author Fernando Montes de Oca Cspedes.
*	@version 1.0
*	@date	29/Jul/2007
*	@brief Esta clase genera una textura de nubes y no permite
*	cambios de cover
*/
class CloudsEstaticas : public Clouds {
public:
    CloudsEstaticas();

    CloudsEstaticas(float cover, float sharpness, int num_octavos, int width, int height);

    ~CloudsEstaticas();

    void forceUpdate();

private:
    void generaTextura();

};

#endif

