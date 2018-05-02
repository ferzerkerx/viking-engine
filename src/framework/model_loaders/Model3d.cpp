#include "Model3d.h"

Model3d::~Model3d() {
    int i = 0;
    while (!objects_.empty()) {
        i = static_cast<int>(objects_.size() - 1);
        delete objects_[i].materials_ids;
        delete objects_[i].material_vert_index;
        delete objects_[i].vertex;
        delete objects_[i].normal;
        delete objects_[i].texture_st;
        delete objects_[i].faces;
        objects_.pop_back();
    }

    while (!materials_.empty()) {
        materials_.pop_back();
    }
}

void Model3d::AddObject(Object3D obj) {
    objects_.push_back(obj);
}

void Model3d::AddMaterial(Material mat) {
    materials_.push_back(mat);
}

std::vector<Material> &Model3d::materials() {
    return materials_;
}

std::vector<Object3D> &Model3d::objects() {
    return objects_;
}
