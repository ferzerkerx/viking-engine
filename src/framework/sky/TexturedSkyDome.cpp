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
    for (i = 0; i < num_vertices_; i++) {
        glTexCoord2f(vrtex_[i].uv.x, vrtex_[i].uv.y);
        glVertex3f(vrtex_[i].pos.x, vrtex_[i].pos.y, vrtex_[i].pos.z);
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
    vector3f vec_temp;

    int i = 0;
    int phi = 0;
    int theta = 0;

    auto dtetha_aux = static_cast<int>(dtheta_);

    for (; phi <= 90 - dphi_; phi += (int) dphi_) {
        for (theta = 0; theta <= 360 - dtetha_aux; theta += dtetha_aux) {

            vec_temp.x = vrtex_[i].pos.x;
            vec_temp.y = vrtex_[i].pos.y;
            vec_temp.z = vrtex_[i].pos.z;

            Normalize(vec_temp);

            vrtex_[i].uv.x = h_tile_ * (std::atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            vrtex_[i].uv.y = v_tile_ * (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            vec_temp.x = vrtex_[i].pos.x;
            vec_temp.y = vrtex_[i].pos.y;
            vec_temp.z = vrtex_[i].pos.z;

            Normalize(vec_temp);

            vrtex_[i].uv.x = h_tile_ * (std::atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            vrtex_[i].uv.y = v_tile_ * (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            vec_temp.x = vrtex_[i].pos.x;
            vec_temp.y = vrtex_[i].pos.y;
            vec_temp.z = vrtex_[i].pos.z;

            Normalize(vec_temp);

            vrtex_[i].uv.x = h_tile_ * (std::atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            vrtex_[i].uv.y = v_tile_ * (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            if (phi > -90 && phi < 90) {
                vec_temp.x = vrtex_[i].pos.x;
                vec_temp.y = vrtex_[i].pos.y;
                vec_temp.z = vrtex_[i].pos.z;

                Normalize(vec_temp);

                vrtex_[i].uv.x = h_tile_ * (std::atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
                vrtex_[i].uv.y = v_tile_ * (asinf(vec_temp.y) / PI) + 0.5F;
                i++;
            }
        }
    }

    AdjustUVRanges();
}

void TexturedSkyDome::AdjustUVRanges() const {
    for (int j = 0; j < num_vertices_ - 3; j++) {

        if (vrtex_[j].uv.x - vrtex_[j + 1].uv.x > 0.9F) {
            vrtex_[j + 1].uv.x += 1.0F;
        }

        if (vrtex_[j + 1].uv.x - vrtex_[j].uv.x > 0.9F) {
            vrtex_[j].uv.x += 1.0F;
        }

        if (vrtex_[j].uv.x - vrtex_[j + 2].uv.x > 0.9F) {
            vrtex_[j + 2].uv.x += 1.0F;
        }

        if (vrtex_[j + 2].uv.x - vrtex_[j].uv.x > 0.9F) {
            vrtex_[j].uv.x += 1.0F;
        }

        if (vrtex_[j + 1].uv.x - vrtex_[j + 2].uv.x > 0.9F) {
            vrtex_[j + 2].uv.x += 1.0F;
        }

        if (vrtex_[j + 2].uv.x - vrtex_[j + 1].uv.x > 0.9F) {
            vrtex_[j + 1].uv.x += 1.0F;
        }

        if (vrtex_[j].uv.y - vrtex_[j + 1].uv.y > 0.8F) {
            vrtex_[j + 1].uv.y += 1.0F;
        }

        if (vrtex_[j + 1].uv.y - vrtex_[j].uv.y > 0.8F) {
            vrtex_[j].uv.y += 1.0F;
        }

        if (vrtex_[j].uv.y - vrtex_[j + 2].uv.y > 0.8F) {
            vrtex_[j + 2].uv.y += 1.0F;
        }

        if (vrtex_[j + 2].uv.y - vrtex_[j].uv.y > 0.8F) {
            vrtex_[j].uv.y += 1.0F;
        }

        if (vrtex_[j + 1].uv.y - vrtex_[j + 2].uv.y > 0.8F) {
            vrtex_[j + 2].uv.y += 1.0F;
        }

        if (vrtex_[j + 2].uv.y - vrtex_[j + 1].uv.y > 0.8F) {
            vrtex_[j + 1].uv.y += 1.0F;
        }
    }
}