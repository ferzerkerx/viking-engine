#include "TexturedSkyDome.h"
#include <GL/glut.h>
#include <cmath>

TexturedSkyDome::TexturedSkyDome(float r, unsigned int texture_id) : SkyDome(r), texture_id_(texture_id) {
    v_tile_ = 1.0F;
    h_tile_ = 1.0F;

    CalculateUV();
}


TexturedSkyDome::TexturedSkyDome(float dphi, float dtheta, float r, unsigned int texture_id) :
        SkyDome(dphi, dtheta, r), texture_id_(texture_id) {
    v_tile_ = 1.0F;
    h_tile_ = 1.0F;

    CalculateUV();
}


TexturedSkyDome::~TexturedSkyDome() = default;


void TexturedSkyDome::set_texture_id(unsigned int texture_id) {
    texture_id_ = texture_id;
}


void TexturedSkyDome::RenderDome(vector3f camera_position) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    int i = 0;
    glPushMatrix();
    glTranslatef(camera_position.x, camera_position.y, camera_position.z);
    glRotatef(-90.0F, 1.0, 0.0, 0.0F);
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i < num_vertices(); i++) {
        glTexCoord2f(vertex_dome()[i].uv.x, vertex_dome()[i].uv.y);
        glVertex3f(vertex_dome()[i].pos.x, vertex_dome()[i].pos.y, vertex_dome()[i].pos.z);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}


void TexturedSkyDome::Render(vector3f camera_position) {
    RenderDome(camera_position);
    RenderSun(camera_position);
}

void TexturedSkyDome::Update(vector3f camera_position) {
    Render(camera_position);
}

void TexturedSkyDome::CalculateUV() {
    int i = 0;
    int phi = 0;
    int theta = 0;

    auto dtetha_aux = static_cast<int>(dtheta());
    auto dphi_aux = static_cast<int>(dphi());

    for (; phi <= 90 - dphi_aux; phi += dphi_aux) {
        for (theta = 0; theta <= 360 - dtetha_aux; theta += dtetha_aux) {
            for(int k =0; k < 3; k++ ) {
                CalculateUVForPolygon(i);
                i++;
            }

            if (phi > -90 && phi < 90) {
                CalculateUVForPolygon(i);
                i++;
            }
        }
    }

    AdjustUVRanges();
}

void TexturedSkyDome::CalculateUVForPolygon(int i) const {
    vector3f vec_temp;
    vec_temp.x = vertex_dome()[i].pos.x;
    vec_temp.y = vertex_dome()[i].pos.y;
    vec_temp.z = vertex_dome()[i].pos.z;

    Normalize(vec_temp);

    vertex_dome()[i].uv.x = h_tile_ * (std::atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
    vertex_dome()[i].uv.y = v_tile_ * (asinf(vec_temp.y) / PI) + 0.5F;
}

void TexturedSkyDome::AdjustUVRanges() const {
    for (int j = 0; j < num_vertices() - 3; j++) {

        if (vertex_dome()[j].uv.x - vertex_dome()[j + 1].uv.x > 0.9F) {
            vertex_dome()[j + 1].uv.x += 1.0F;
        }

        if (vertex_dome()[j + 1].uv.x - vertex_dome()[j].uv.x > 0.9F) {
            vertex_dome()[j].uv.x += 1.0F;
        }

        if (vertex_dome()[j].uv.x - vertex_dome()[j + 2].uv.x > 0.9F) {
            vertex_dome()[j + 2].uv.x += 1.0F;
        }

        if (vertex_dome()[j + 2].uv.x - vertex_dome()[j].uv.x > 0.9F) {
            vertex_dome()[j].uv.x += 1.0F;
        }

        if (vertex_dome()[j + 1].uv.x - vertex_dome()[j + 2].uv.x > 0.9F) {
            vertex_dome()[j + 2].uv.x += 1.0F;
        }

        if (vertex_dome()[j + 2].uv.x - vertex_dome()[j + 1].uv.x > 0.9F) {
            vertex_dome()[j + 1].uv.x += 1.0F;
        }

        if (vertex_dome()[j].uv.y - vertex_dome()[j + 1].uv.y > 0.8F) {
            vertex_dome()[j + 1].uv.y += 1.0F;
        }

        if (vertex_dome()[j + 1].uv.y - vertex_dome()[j].uv.y > 0.8F) {
            vertex_dome()[j].uv.y += 1.0F;
        }

        if (vertex_dome()[j].uv.y - vertex_dome()[j + 2].uv.y > 0.8F) {
            vertex_dome()[j + 2].uv.y += 1.0F;
        }

        if (vertex_dome()[j + 2].uv.y - vertex_dome()[j].uv.y > 0.8F) {
            vertex_dome()[j].uv.y += 1.0F;
        }

        if (vertex_dome()[j + 1].uv.y - vertex_dome()[j + 2].uv.y > 0.8F) {
            vertex_dome()[j + 2].uv.y += 1.0F;
        }

        if (vertex_dome()[j + 2].uv.y - vertex_dome()[j + 1].uv.y > 0.8F) {
            vertex_dome()[j + 1].uv.y += 1.0F;
        }
    }
}

unsigned int TexturedSkyDome::texture_id() const {
    return texture_id_;
}

float TexturedSkyDome::h_tile() const {
    return h_tile_;
}

float TexturedSkyDome::v_tile() const {
    return v_tile_;
}
