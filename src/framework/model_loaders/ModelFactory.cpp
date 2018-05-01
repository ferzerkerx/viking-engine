//
// Created by fernando on 01.05.18.
//

#include "ModelFactory.h"
#include "MD2Loader.h"

Md2Model *ModelFactory::LoadMD2(const char *md2_file_name, const char *texture_file_name) {
    auto *md2_loader = new MD2Loader();
    Model3d *loaded_model = md2_loader->load(md2_file_name, texture_file_name);
    delete md2_loader;

    return dynamic_cast<Md2Model *>(loaded_model);
}
