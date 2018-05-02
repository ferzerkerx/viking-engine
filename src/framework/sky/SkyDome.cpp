#include <GL/gl.h>
#include "SkyDome.h"


SkyDome::SkyDome(float r) : r_(r) {
    dphi_ = 5.0F;
    dtheta_ = 10.0F;
    has_sun_texture_ = false;
    sun_texture_ = 0;

    vrtex_ = nullptr;
    CreateHemiSphere();
    sun_polar_ = polar3f(1, 0, r_ - 2);
}

SkyDome::SkyDome(float dphi, float dtheta, float r) : dphi_(dphi), dtheta_(dtheta), r_(r) {
    has_sun_texture_ = false;
    sun_texture_ = 0;

    vrtex_ = nullptr;
    CreateHemiSphere();
    sun_polar_ = polar3f(0, 0, r_ - 5);
}


SkyDome::~SkyDome() {
    delete[] vrtex_;
    if (has_sun_texture_) { glDeleteTextures(1, &sun_texture_); }
}

void SkyDome::CreateHemiSphere() {
    num_vertices_ = (int) ((360.0F / dtheta_) * (90.0F / dphi_) *
                        GL_TRIANGLE_STRIP);

    vrtex_ = new VertexDome[num_vertices_];

    int i = 0;
    int phi = 0;
    int theta = 0;
    float phirad;
    float thetarad;
    float phidrad;
    float thetadrad;

    auto dtetha_aux = static_cast<int>(dtheta_);

    for (; phi <= 90 - dphi_; phi += (int) dphi_) {
        for (theta = 0; theta <= 360 - dtetha_aux; theta += dtetha_aux) {
            phirad = phi * RAD;
            thetarad = theta * RAD;
            phidrad = (phi + dphi_) * RAD;
            thetadrad = (theta + dtheta_) * RAD;

            vrtex_[i].pos = vector3f(r_ * sinf(phirad) * cosf(thetarad),
                                     r_ * sinf(phirad) * sinf(thetarad),
                                     r_ * cosf(phirad));
            vrtex_[i].pol = polar3f(phi, theta, r_);
            i++;

            vrtex_[i].pos = vector3f(r_ * sinf(phidrad) * cosf(thetarad),
                                     r_ * sinf(phidrad) * sinf(thetarad),
                                     r_ * cosf(phidrad));
            vrtex_[i].pol = polar3f(phi + dphi_, theta, r_);
            i++;

            vrtex_[i].pos = vector3f(r_ * sinf(phirad) * cosf(thetadrad),
                                     r_ * sinf(phirad) * sinf(thetadrad),
                                     r_ * cosf(phirad));
            vrtex_[i].pol = polar3f(phi, theta + dtheta_, r_);
            i++;

            if (phi > -90 && phi < 90) {
                vrtex_[i].pos = vector3f(r_ * sinf(phidrad) * cosf(thetadrad),
                                         r_ * sinf(phidrad) * sinf(thetadrad),
                                         r_ * cosf(phidrad));
                vrtex_[i].pol = polar3f(phi + dphi_, theta + dtheta_, r_);
                i++;
            }
        }
    }
}

void SkyDome::set_sun_texture(unsigned int texture_id) {
    sun_texture_ = texture_id;
    has_sun_texture_ = true;
}

void SkyDome::RenderSun(vector3f camera_position) {
    if (has_sun_texture_) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sun_texture_);
        glPushMatrix();
        glTranslatef(camera_position.x, camera_position.y, camera_position.z);
        sun_vector_ = vector3f(sun_polar_);
        sun_polar_.phi += 0.002F;
        glTranslatef(sun_vector_.y, sun_vector_.x, sun_vector_.z);
        glRotatef(-90.0F, 1.0, 0.0, 0.0F);
        Billboard::BBSphere(camera_position, sun_vector_.y, sun_vector_.x, sun_vector_.z);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-5.0F, 0.0, -5.0F);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(5.0, 0.0, -5.0F);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(5.0, 0.0, 5.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-5.0F, 0.0, 5.0);
        glEnd();
        Billboard::BBFin();
        glPopMatrix();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}