#include "ColoredSkyDome.h"
#include <GL/glut.h>

ColoredSkyDome::ColoredSkyDome(float r) : SkyDome(r) {
    ColorDome(vertex_dome(), num_vertices())
}


ColoredSkyDome::ColoredSkyDome(float dphi, float dtheta, float r) : SkyDome(dphi, dtheta, r) {
    ColorDome(vertex_dome(), num_vertices());
}

static void ColoredSkyDome::ColorDome(VertexDome *vertexDome, int num_verices) const {
    for (int i = 0; i < num_verices; i++) {
        vertexDome[i].color = color3f(0.0, 0.35F, 0.7F);
    }
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
    for (i = 0; i < num_vertices(); i++) {
        glColor3f(vertex_dome()[i].color.r, vertex_dome()[i].color.g, vertex_dome()[i].color.b);
        glVertex3f(vertex_dome()[i].pos.x, vertex_dome()[i].pos.y, vertex_dome()[i].pos.z);
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