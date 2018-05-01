//
// Created by fernando on 01.05.18.
//

#ifndef VIKING_ENGINE_MODELFACTORY_H
#define VIKING_ENGINE_MODELFACTORY_H


#include "MD2Model.h"

class ModelFactory {

public:
    static MD2Model* loadMD2(const char *modelo, const char *text);
};


#endif //VIKING_ENGINE_MODELFACTORY_H
