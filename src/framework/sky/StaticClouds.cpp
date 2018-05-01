#include <cstdio>
#include "StaticClouds.h"


StaticClouds::StaticClouds() : Clouds() {
    CreateTexture();
}


StaticClouds::StaticClouds(float cover, float sharpness, int num_octavos, int width, int height) : Clouds(cover,
                                                                                                          sharpness,
                                                                                                          num_octavos,
                                                                                                          width,
                                                                                                          height) {
    CreateTexture();
}


StaticClouds::~StaticClouds() = default;

void StaticClouds::CreateTexture() {

    map_ = new float[text_width_ * text_height_];
    RGBA_text_ = new char[text_width_ * text_height_ * 4];

    Perlin::SetNoise(map32_);
    Perlin::OverlapOctaves(map32_, map_, text_width_, text_height_);
    Perlin::expFilter(map_, text_width_, text_height_, cover_, sharpness_);

    Clouds::GeneraGLTexture();

    if (map_ != nullptr) {
        delete[] map_;
        map_ = nullptr;
    }
    if (RGBA_text_ != nullptr) {
        delete[] RGBA_text_;
        RGBA_text_ = nullptr;
    }
}

void StaticClouds::ForceUpdate() {
    CreateTexture();
}