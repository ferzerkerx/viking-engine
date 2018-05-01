
#include "ColoredSkyDome.h"
#include <cstdio>
#include <GL/glut.h>


ColoredSkyDome::ColoredSkyDome(float r) : SkyDome(r) {
    ColorDome();
}



ColoredSkyDome::ColoredSkyDome(float dphi, float dtheta, float r) : SkyDome(dphi, dtheta, r) {
    ColorDome();
}

ColoredSkyDome::~ColoredSkyDome() = default;


void ColoredSkyDome::RenderDome(vector3f camera_position) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    int i = 0;
    glPushMatrix();
    glTranslatef(camera_position.x, camera_position.y, camera_position.z);
    glRotatef(-90.0F, 1.0, 0.0, 0.0F);
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i < num_vertices_; i++) {
        glColor3f(vrtex_[i].color.r, vrtex_[i].color.g, vrtex_[i].color.b);
        glVertex3f(vrtex_[i].pos.x, vrtex_[i].pos.y, vrtex_[i].pos.z);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_CULL_FACE);

    glColor3f(1.0F, 1.0F, 1.0F);
}


void ColoredSkyDome::Render(vector3f camera_position) {
    RenderDome(camera_position);
    RenderSun(camera_position);
}


void ColoredSkyDome::Update(vector3f camera_position) {
    Render(camera_position);
}

void ColoredSkyDome::ColorDome() {
    for (int i = 0; i < num_vertices_; i++) {
        vrtex_[i].color = color3f(0.0, 0.35F, 0.7F);
    }
}