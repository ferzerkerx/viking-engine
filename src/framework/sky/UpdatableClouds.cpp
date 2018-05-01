#include <GL/glut.h>
#include <cstdio>
#include "UpdatableClouds.h"


UpdatableClouds::UpdatableClouds() : Clouds() {
    texture_update_time_in_millis_ = 500;
    brightness_ = 0.0F;
    current_octave_ = 0;

    octaves_ = new struct octave[num_octaves_];
    for (int i = 0; i < num_octaves_; i++) {
        octaves_[i].scale = 1 / pow(2, 3 - i);
        octaves_[i].x = 0;
    }

    map_ = new float[text_width_ * text_height_];
    RGBA_text_ = new char[text_width_ * text_height_ * 4];

    Perlin::setNoise(map32_);
    Perlin::overlapOctaves(map32_, map_, text_width_, text_height_);
    Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);
    CreateTexture();

}

UpdatableClouds::UpdatableClouds(float cover, float sharpness, int num_octaves, int width, int height,
                                         unsigned int up_text_milli)
        : Clouds(cover, sharpness, num_octaves, width, height),
          current_octave_(static_cast<unsigned char>(up_text_milli)) {
    brightness_ = 0.0F;
    current_octave_ = 0;

    octaves_ = new struct octave[num_octaves_];
    for (int i = 0; i < num_octaves_; i++) {
        octaves_[i].scale = static_cast<float>(1 / pow(2, 3 - i));
        octaves_[i].x = 0;
    }

    map_ = new float[text_width_ * text_height_];
    RGBA_text_ = new char[text_width_ * text_height_ * 4];

    Perlin::setNoise(map32_);
    Perlin::overlapOctaves(map32_, map_, text_width_, text_height_);
    Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);
    CreateTexture();
}

UpdatableClouds::~UpdatableClouds() {
    delete[] octaves_;
}

void UpdatableClouds::ForceUpdate() {
    Perlin::overlapOctaves(map32_, map_, text_width_, text_height_);
    Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);
    current_octave_ = 0;
    timer_.reset();
}

void UpdatableClouds::set_texture_update_time(int milliseconds) {
    texture_update_time_in_millis_ = static_cast<unsigned int>(milliseconds);
}

void UpdatableClouds::set_num_octaves(unsigned char number) {
    num_octaves_ = number;
    delete[] octaves_;

    octaves_ = new struct octave[num_octaves_];
    for (int i = 0; i < num_octaves_; i++) {
        octaves_[i].scale = static_cast<float>(1 / pow(2, 3 - i));
        octaves_[i].x = 0;
    }
}

void UpdatableClouds::set_cover(float cover) {
    cover_ = cover;
}

void UpdatableClouds::set_sharpness(float sharpness) {
    sharpness_ = sharpness;
}


void UpdatableClouds::set_texture_resolution(int width, int height) {
    if (width < 32 || height < 32) {
        return;
    }
    delete[] RGBA_text_;
    delete[] map_;

    text_width_ = width;
    text_height_ = height;

    octaves_[current_octave_].x = 0;
    map_ = new float[text_width_ * text_height_ * 4];
    RGBA_text_ = new char[text_width_ * text_height_ * 4];
    current_octave_ = 0;
    Perlin::resetOctaves(map_, text_width_, text_height_);
}

int UpdatableClouds::UpdatePerOctave() {
    int current_millis = timer_.getMilliseconds();

    if ((current_octave_ < num_octaves_) &&
        (current_millis >= (texture_update_time_in_millis_ / (num_octaves_ + 1)) * current_octave_)) {
        Perlin::overlapOctave(map32_, map_, text_width_, text_height_, current_octave_,
                              octaves_[current_octave_].scale);
        current_octave_++;
    }

    if ((current_octave_ == num_octaves_) && (timer_.getMilliseconds() >= texture_update_time_in_millis_)) {
        timer_.reset();
        Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);
        current_octave_ = 0;
        CreateTexture();
        return 1;
    }

    return 0;
}

int UpdatableClouds::UpdatePerCoordinate() {
    int current_millis = timer_.getMilliseconds();

    if ((current_octave_ < num_octaves_) &&
        (current_millis >= (texture_update_time_in_millis_ / (num_octaves_ + 1)) * current_octave_)) {

        for (int y = 0; y < text_height_; y++) {
            brightness_ = Perlin::interpolate(octaves_[current_octave_].x * octaves_[current_octave_].scale,
                                               y * octaves_[current_octave_].scale, map32_);
            map_[(y * text_height_) + octaves_[current_octave_].x] += brightness_ / (1 << current_octave_);

        }

        if (octaves_[current_octave_].x < text_width_) {
            octaves_[current_octave_].x++;
        } else {
            octaves_[current_octave_].x = 0;
            current_octave_++;
        }
    }

    if ((current_octave_ == num_octaves_) && timer_.getMilliseconds() >= texture_update_time_in_millis_) {
        timer_.reset();
        Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);
        current_octave_ = 0;
        CreateTexture();
        return 1;
    }

    return 0;
}

void UpdatableClouds::CreateTexture() {
    Clouds::generaTexturaGL();
    Perlin::resetOctaves(map_, text_width_, text_height_);
}