#include <GL/glut.h>
#include <cstdio>
#include "UpdatableClouds.h"


UpdatableClouds::UpdatableClouds() : Clouds() {
    texture_update_time_in_millis_ = 500;
    brightness_ = 0.0F;
    current_octave_ = 0;

    octaves_ = new struct octave[num_octaves()];
    for (int i = 0; i < num_octaves(); i++) {
        octaves_[i].scale = 1 / pow(2, 3 - i);
        octaves_[i].x = 0;
    }

    Clouds::set_map(new float[text_width() * text_height()]);
    Clouds::set_RGBA_text(new char[text_width() * text_height() * 4]);

    Perlin::SetNoise(map32());
    Perlin::OverlapOctaves(map32(), map(), text_width(), text_height());
    Perlin::expFilter(map(), text_width(), text_height(), cover(), sharpness());
    CreateTexture();

}

UpdatableClouds::UpdatableClouds(float cover, float sharpness, int num_octaves, int width, int height,
                                         unsigned int up_text_milli)
        : Clouds(cover, sharpness, num_octaves, width, height),
          current_octave_(static_cast<unsigned char>(up_text_milli)) {
    brightness_ = 0.0F;
    current_octave_ = 0;

    octaves_ = new struct octave[num_octaves];
    for (int i = 0; i < num_octaves; i++) {
        octaves_[i].scale = static_cast<float>(1 / pow(2, 3 - i));
        octaves_[i].x = 0;
    }

    set_map(new float[text_width() * text_height()]);
    set_RGBA_text(new char[text_width() * text_height() * 4]);

    Perlin::SetNoise(map32());
    Perlin::OverlapOctaves(map32(), map(), text_width(), text_height());
    Perlin::expFilter(map(), text_width(), text_height(), cover, sharpness);
    CreateTexture();
}

UpdatableClouds::~UpdatableClouds() {
    delete[] octaves_;
}

void UpdatableClouds::ForceUpdate() {
    Perlin::OverlapOctaves(map32(), map(), text_width(), text_height());
    Perlin::expFilter(map(), text_width(), text_height(), cover(), sharpness());
    current_octave_ = 0;
    timer_.Reset();
}

void UpdatableClouds::set_texture_update_time(int milliseconds) {
    texture_update_time_in_millis_ = static_cast<unsigned int>(milliseconds);
}

void UpdatableClouds::set_num_octaves(unsigned char number) {
    Clouds::set_num_octaves(number);
    delete[] octaves_;

    octaves_ = new struct octave[num_octaves()];
    for (int i = 0; i < num_octaves(); i++) {
        octaves_[i].scale = static_cast<float>(1 / pow(2, 3 - i));
        octaves_[i].x = 0;
    }
}

void UpdatableClouds::set_texture_resolution(int width, int height) {
    if (width < 32 || height < 32) {
        return;
    }
    delete[] RGBA_text();
    delete[] map();

    set_texture_resolution(width, height);

    octaves_[current_octave_].x = 0;
    set_map(new float[text_width() * text_height() * 4]);
    set_RGBA_text(new char[text_width() * text_height() * 4]);
    current_octave_ = 0;
    Perlin::ResetOctaves(map(), text_width(), text_height());
}

int UpdatableClouds::UpdatePerOctave() {
    int current_millis = timer_.Milliseconds();

    if ((current_octave_ < num_octaves()) &&
        (current_millis >= (texture_update_time_in_millis_ / (num_octaves() + 1)) * current_octave_)) {
        Perlin::OverlapOctave(map32(), map(), text_width(), text_height(), current_octave_,
                              octaves_[current_octave_].scale);
        current_octave_++;
    }

    if ((current_octave_ == num_octaves()) && (timer_.Milliseconds() >= texture_update_time_in_millis_)) {
        timer_.Reset();
        Perlin::expFilter(map(), text_width(), text_height(), cover(), sharpness());
        current_octave_ = 0;
        CreateTexture();
        return 1;
    }

    return 0;
}

int UpdatableClouds::UpdatePerCoordinate() {
    int current_millis = timer_.Milliseconds();

    if ((current_octave_ < num_octaves()) &&
        (current_millis >= (texture_update_time_in_millis_ / (num_octaves() + 1)) * current_octave_)) {

        for (int y = 0; y < text_height(); y++) {
            brightness_ = Perlin::Interpolate(octaves_[current_octave_].x * octaves_[current_octave_].scale,
                                              y * octaves_[current_octave_].scale, map32());
            map()[(y * text_height()) + octaves_[current_octave_].x] += brightness_ / (1 << current_octave_);

        }

        if (octaves_[current_octave_].x < text_width()) {
            octaves_[current_octave_].x++;
        } else {
            octaves_[current_octave_].x = 0;
            current_octave_++;
        }
    }

    if ((current_octave_ == num_octaves()) && timer_.Milliseconds() >= texture_update_time_in_millis_) {
        timer_.Reset();
        Perlin::expFilter(map(), text_width(), text_height(), cover(), sharpness());
        current_octave_ = 0;
        CreateTexture();
        return 1;
    }

    return 0;
}

void UpdatableClouds::CreateTexture() {
    Clouds::GeneraGLTexture();
    Perlin::ResetOctaves(map(), text_width(), text_height());
}