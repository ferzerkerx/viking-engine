//
// Created by fernando on 01.05.18.
//

#ifndef VIKING_ENGINE_MODELFACTORY_H
#define VIKING_ENGINE_MODELFACTORY_H


#include "Md2Model.h"

class ModelFactory {

public:
    static Md2Model *LoadMD2(const char *md2_file_name, const char *texture_file_name);
};


#endif //VIKING_ENGINE_MODELFACTORY_H
