//
// Created by fernando on 01.05.18.
//

#include "ModelFactory.h"
#include "MD2Loader.h"

MD2Model *ModelFactory::loadMD2(const char *modelo, const char *text) {
    auto *m_ml = new MD2Loader();
    Model3D *loadedModel = m_ml->importar(modelo, text);
    delete m_ml;

    return dynamic_cast<MD2Model *>(loadedModel);
}
