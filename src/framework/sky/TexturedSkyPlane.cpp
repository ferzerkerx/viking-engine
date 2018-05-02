#include "TexturedSkyPlane.h"
#include <GL/glut.h>
#include <cmath>


TexturedSkyPlane::TexturedSkyPlane(int divisiones, float planet_radius, float atmosphere_radius, float h_tile,
                                   float v_tile, unsigned int texture) {
    divisions_ = static_cast<unsigned int>(divisiones);
    if (divisions_ < 1) {
        divisions_ = 1;
    }
    if (divisions_ > 256) {
        divisions_ = 256;
    }

    m_texture = texture;

    planet_radius_ = planet_radius;
    atmosphere_radius_ = atmosphere_radius;
    num_vertex_ = (divisions_ + 1) * (divisions_ + 1);
    num_indexes_ = divisions_ * divisions_ * 2 * 3;
    indexes_ = new unsigned int[num_indexes_];
    vertex_ = new vertx[num_vertex_];
    roty_factor_ = 0.006F;

    m_h_tile = h_tile;
    m_v_tile = v_tile;

    wind_factor_x_ = 0.0001F;
    wind_factor_y_ = 0.0001F;
    texture_delta_x = 0.0F;
    delta_texture_y_ = 0.0F;
    rot_texture_in_milli_ = 10;

    GenerateSkyPlane();
}


TexturedSkyPlane::~TexturedSkyPlane() {
    delete[] indexes_;
    delete[] vertex_;
}


void TexturedSkyPlane::Update(vector3f camera_position) {
    Render(camera_position);
}


void TexturedSkyPlane::Render(vector3f poscam) {
    if (timer_.Milliseconds() >= rot_texture_in_milli_) {
        timer_.Reset();
        texture_delta_x += roty_factor_;
        if (texture_delta_x >= 360) {
            texture_delta_x -= 360;
        }
    }

    glPushMatrix();
    glTranslatef(poscam.x, poscam.y + planet_radius_, poscam.z);
    glRotatef(texture_delta_x, 0.0, 1.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < num_indexes_; i++) {
        glTexCoord2f(vertex_[indexes_[i]].u, vertex_[indexes_[i]].v);
        glVertex3f(vertex_[indexes_[i]].pos.x, vertex_[indexes_[i]].pos.y, vertex_[indexes_[i]].pos.z);
    }

    glEnd();


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glPopMatrix();
}



void TexturedSkyPlane::GenerateSkyPlane() {

    float plane_size = 2.0F * std::sqrt((SQR(atmosphere_radius_) - SQR(planet_radius_)));
    float delta = plane_size / (float) divisions_;
    float tex_delta = 2.0F / (float) divisions_;

    float x_dist;
    float z_dist;
    float x_height;
    float z_height;
    float height;

    int i = 0;
    int j = 0;

    vertx sv;

    for (i = 0; i <= divisions_; i++) {
        for (j = 0; j <= divisions_; j++) {
            x_dist = (-0.5F * plane_size) + ((float) j * delta);
            z_dist = (-0.5F * plane_size) + ((float) i * delta);

            x_height = (x_dist * x_dist) / atmosphere_radius_;
            z_height = (z_dist * z_dist) / atmosphere_radius_;
            height = x_height + z_height;

            sv.pos.x = x_dist;
            sv.pos.y = 0.0F - height;
            sv.pos.z = z_dist;

            sv.u = m_h_tile * ((float) j * tex_delta * 0.5F);
            sv.v = m_v_tile * (1.0F - (float) i * tex_delta * 0.5F);

            vertex_[i * (divisions_ + 1) + j] = sv;
        }
    }

    int index = 0;
    for (i = 0; i < divisions_; i++) {
        for (j = 0; j < divisions_; j++) {
            unsigned int start_vert = (i * (divisions_ + 1) + j);

            indexes_[index++] = start_vert;
            indexes_[index++] = start_vert + 1;
            indexes_[index++] = start_vert + divisions_ + 1;

            indexes_[index++] = start_vert + 1;
            indexes_[index++] = start_vert + divisions_ + 2;
            indexes_[index++] = start_vert + divisions_ + 1;
        }
    }
}


void TexturedSkyPlane::set_wind_factor_x(float wfx) {
    wind_factor_x_ = wfx;
}


void TexturedSkyPlane::set_wind_factor_y(float wfy) {
    wind_factor_y_ = wfy;
}

void TexturedSkyPlane::set_wind_factor_XY(float wfx, float wfy) {
    wind_factor_x_ = wfx;
    wind_factor_y_ = wfy;
}