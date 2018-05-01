#ifndef __CLOUDS_ACT_H__
#define __CLOUDS_ACT_H__

#include "Clouds.h"
#include "../Timer.h"


struct octave {
    int x;
    double scale;
};

/**
* @class UpdatableClouds
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 5:12:31 PM
*/

class UpdatableClouds : public Clouds {

public:
    UpdatableClouds();

    UpdatableClouds(float cover, float sharpness, int num_octaves, int width, int height,
                        unsigned int up_text_milli);

    ~UpdatableClouds() override;

    int UpdatePerOctave();

    int UpdatePerCoordinate();

    void ForceUpdate() override;

    void set_texture_update_time(int milliseconds);

    void set_num_octaves(unsigned char number);

    void set_cover(float cover);

    void set_sharpness(float sharpness);

    void set_texture_resolution(int width, int height);

private:
    Timer timer_;
    float brightness_;
    unsigned int texture_update_time_in_millis_;
    unsigned char current_octave_;

    octave *octaves_;

    void CreateTexture() override;

};

#endif