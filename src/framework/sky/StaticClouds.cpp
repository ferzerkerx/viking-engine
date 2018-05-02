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
    set_map(new float[text_width() * text_height()]);
    set_RGBA_text(new char[text_width() * text_height() * 4]);

    Perlin::SetNoise(map32());
    Perlin::OverlapOctaves(map32(), map(), text_width(), text_height());
    Perlin::expFilter(map(), text_width(), text_height(), cover(), sharpness());

    Clouds::GeneraGLTexture();

    if (map() != nullptr) {
        delete[] map();
        set_map(nullptr);
    }
    if (RGBA_text() != nullptr) {
        delete[] RGBA_text();
        set_RGBA_text(nullptr);
    }
}

void StaticClouds::ForceUpdate() {
    CreateTexture();
}