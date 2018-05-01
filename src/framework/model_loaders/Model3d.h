/**
* @file Model3D.h
* @brief Encabezado de la clase Model3D
* @date Saturday, November 03, 2007 11:14:28 AM
*/
#ifndef __MODEL3D_H__
#define __MODEL3D_H__

#include <vector>
#include "../math/vector.h"


struct Animation {
    char name[255];
    int initial_frame;
    int final_frame;
};

struct Material {
    char name[255]{};
    color3f color;
    int texture_id{};
    float u_tile{};
    float v_tile{};
};

struct Face {
    int vert_index[3];
    int st_index[3];
};

struct Object3D {
    int num_verts;
    int num_faces;
    int num_st;
    int *materials_ids;
    int *material_vert_index;
    vector3f *vertex;
    vector3f *normal;
    vector2f *texture_st;
    Face *faces;
};

/**
* @class Model3D
* @author Fernando Montes de Oca Cespedes
* @date Friday, November 23, 2007 6:16:49 PM
*/
class Model3d {

protected:
    std::vector<Material> materials_;
    std::vector<Object3D> objects;

public:
    virtual ~Model3d();

    void AddObject(Object3D obj);

    void AddMaterial(Material mat);

    virtual void Render()=0;

    virtual void Update()=0;

};

#endif