#include <GL/glut.h>
#include "Clouds.h"


Clouds::Clouds() {
    cover_ = 45.0F;
    sharpness_ = 0.987F;
    num_octaves_ = 4;
    texture_id_ = 0;

    text_width_ = 384;
    text_height_ = 384;

}


Clouds::Clouds(float cover, float sharpness, int num_octavos, int width, int height) : cover_(cover),
                                                                                       sharpness_(sharpness),
                                                                                       text_width_(width),
                                                                                       text_height_(height) {
    num_octaves_ = static_cast<unsigned char>(num_octavos);
    texture_id_ = 0;
}


Clouds::~Clouds() {
    delete[] map_;
    delete[] RGBA_text_;

    if (texture_id_ != -1) { glDeleteTextures(1, &texture_id_); }

}


int Clouds::texture_width() {
    return text_width_;
}


int Clouds::texture_height() {
    return text_height_;
}


unsigned int Clouds::texture_id() {
    return texture_id_;
}

void Clouds::CalculateAlpha() {
    float color;
    int desplazamiento = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < text_width_; i++) {
        for (j = 0; j < text_height_; j++) {
            color = map_[i * text_width_ + j];
            desplazamiento = i * (text_height_ * 4) + (j * 4);
            RGBA_text_[desplazamiento] = static_cast<char>(color);
            RGBA_text_[desplazamiento + 1] = static_cast<char>(color);
            RGBA_text_[desplazamiento + 2] = static_cast<char>(color);
            if (color == 0.0F) {
                RGBA_text_[desplazamiento + 3] = 0;
            } else {
                RGBA_text_[desplazamiento + 3] = static_cast<char>(255);
            }
        }
    }
}

void Clouds::GeneraGLTexture() {

    if (map_ != nullptr && RGBA_text_ != nullptr) {
        CalculateAlpha();

        glDeleteTextures(1, &texture_id_);
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, text_width_, text_height_, GL_RGBA, GL_UNSIGNED_BYTE, RGBA_text_);
    }
}

char *Clouds::getData() {
    return RGBA_text_;
}