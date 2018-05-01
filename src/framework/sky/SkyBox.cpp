#include "SkyBox.h"
#include <GL/glut.h>

SkyBox::SkyBox(float height, float large, float width, unsigned int *faces) {
    if (height <= 0.0F) {
        height = 10.0F;
    }
    if (large <= 0.0F) {
        large = 10.0F;
    }
    if (width <= 0.0F) {
        width = 10.0F;
    }
    height_ = height / 2.0F;
    large_ = large / 2.0F;
    width_ = width / 2.0F;

    if (faces != nullptr) {
        for (int i = 0; i < 6; i++) {
            faces_[i] = faces[i];
            glBindTexture(GL_TEXTURE_2D, faces_[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }
    }
}
SkyBox::SkyBox(float height, float large, float width, unsigned int front,
               unsigned int back, unsigned int left, unsigned int right, unsigned int up, unsigned int down) {
    if (height <= 0.0F) {
        height = 10.0F;
    }
    if (large <= 0.0F) {
        large = 10.0F;
    }
    if (width <= 0.0F) {
        width = 10.0F;
    }
    height_ = height / 2.0F;
    large_ = large / 2.0F;
    width_ = width / 2.0F;

    faces_[FRONT] = front;
    faces_[BACK] = back;
    faces_[LEFT] = left;
    faces_[RIGHT] = right;
    faces_[UP] = up;
    faces_[DOWN] = down;

    for (unsigned int m_cara : faces_) {
        glBindTexture(GL_TEXTURE_2D, m_cara);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
}


SkyBox::~SkyBox() {
    glDeleteTextures(6, faces_);
}

void SkyBox::Update(vector3f camera_position) {
    Render(camera_position);
}


void SkyBox::Render(vector3f camera_position) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glPushMatrix();
    glTranslatef(camera_position.x, camera_position.y, camera_position.z);

    glBindTexture(GL_TEXTURE_2D, faces_[FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(large_, -height_, -width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-large_, -height_, -width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-large_, height_, -width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(large_, height_, -width_);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, faces_[BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-large_, -height_, width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(large_, -height_, width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(large_, height_, width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-large_, height_, width_);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, faces_[LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-large_, -height_, -width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-large_, -height_, width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-large_, height_, width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-large_, height_, -width_);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, faces_[RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(large_, -height_, width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(large_, -height_, -width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(large_, height_, -width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(large_, height_, width_);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, faces_[UP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(large_, height_, -width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-large_, height_, -width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-large_, height_, width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(large_, height_, width_);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, faces_[DOWN]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(large_, -height_, width_);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-large_, -height_, width_);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-large_, -height_, -width_);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(large_, -height_, -width_);
    glEnd();
    glPopMatrix();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}


float SkyBox::height() {
    return height_ * 2;
}

float SkyBox::large() {
    return large_ * 2;
}

float SkyBox::width() {
    return width_ * 2;
}