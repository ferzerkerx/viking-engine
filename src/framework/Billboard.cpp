#include "GL/gl.h"
#include "Billboard.h"
#include <cmath>

void Billboard::BBFalseSphere() {
    float model_view[16];
    int i;
    int j;

    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                model_view[i * 4 + j] = 1.0;
            }
            else {
                model_view[i * 4 + j] = 0.0;
            }
        }
    }
    glLoadMatrixf(model_view);
}


void Billboard::BBSphere(vector3f camera_position, float obj_pos_x, float obj_pos_y, float obj_pos_z) {

    glPushMatrix();

    float cos_angulo;
    vector3f obj_camera_projection = RotateBasedOnCameraPosition(camera_position, obj_pos_x, obj_pos_z);

    vector3f objACam;
    objACam.x = camera_position.x - obj_pos_x;
    objACam.y = camera_position.y - obj_pos_y;
    objACam.z = camera_position.z - obj_pos_z;

    objACam = Normalize(objACam);

    cos_angulo = Point(obj_camera_projection, objACam);

    if ((cos_angulo < 0.9999) && (cos_angulo > -0.9999)) {
        if (objACam.y < 0) {
            glRotatef((float) (std::acos(cos_angulo) * 180 / 3.1416), 1, 0, 0);
        } else {
            glRotatef((float) (std::acos(cos_angulo) * 180 / 3.1416), -1, 0, 0);
        }
    }

}

vector3f Billboard::RotateBasedOnCameraPosition(const vector3f &camera_position, float obj_pos_x, float obj_pos_z) {
    vector3f look_at = vector3f(0, 0, 1);
    vector3f obj_camera_projection = ObjectCameraPositionBasedOnObject(camera_position, obj_pos_x, obj_pos_z);
    vector3f up_aux = Cross(look_at, obj_camera_projection);

    float cos_angulo;
    cos_angulo = Point(look_at, obj_camera_projection);

    if ((cos_angulo < 0.9999) && (cos_angulo > -0.9999)) {
        glRotatef((float) (std::acos(cos_angulo) * 180 / 3.1416), up_aux.x, up_aux.y, up_aux.z);
    }
    return obj_camera_projection;
}

vector3f Billboard::ObjectCameraPositionBasedOnObject(const vector3f &camera_position, float obj_pos_x, float obj_pos_z) {
    vector3f obj_camera_projection;
    obj_camera_projection.x = camera_position.x - obj_pos_x;
    obj_camera_projection.y = 0;
    obj_camera_projection.z = camera_position.z - obj_pos_z;

    obj_camera_projection = Normalize(obj_camera_projection);
    return obj_camera_projection;
}


void Billboard::BBFalseCylinder() {
    float model_view[16];
    int i;
    int j;

    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view);
    for (i = 0; i < 3; i += 2) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                model_view[i * 4 + j] = 1.0;
            } else {
                model_view[i * 4 + j] = 0.0;
            }
        }

    }
    glLoadMatrixf(model_view);
}

void Billboard::BBCylinder(vector3f camera_position, float obj_pos_x, float obj_pos_y, float obj_pos_z) {
    glPushMatrix();
    RotateBasedOnCameraPosition(camera_position, obj_pos_x, obj_pos_z);
}


void Billboard::BBFin() {
    glPopMatrix();
}