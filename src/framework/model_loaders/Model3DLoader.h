#ifndef __MODEL3DLOADER__H__
#define __MODEL3DLOADER__H__

#include "../Textura.h"
#include "Model3d.h"

/**
* @class Model3DLoader
* @author Fernando Montes de Oca Cespedes
* @date Friday, November 23, 2007 5:49:22 PM
*/
class Model3DLoader {

public:
    Model3DLoader() {
    }

    virtual ~Model3DLoader() {
    }

    virtual Model3d *Load(const char *file_name) = 0;
};


#endif