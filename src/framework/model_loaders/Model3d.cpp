#include "Model3d.h"

Model3d::~Model3d() {
    int i = 0;
    while (!objects.empty()) {
        i = static_cast<int>(objects.size() - 1);
        delete objects[i].materials_ids;
        delete objects[i].material_vert_index;
        delete objects[i].vertex;
        delete objects[i].normal;
        delete objects[i].texture_st;
        delete objects[i].faces;
        objects.pop_back();
    }

    while (!materials_.empty()) {
        materials_.pop_back();
    }
}

void Model3d::AddObject(Object3D obj) {
    objects.push_back(obj);
}

void Model3d::AddMaterial(Material mat) {
    materials_.push_back(mat);
}