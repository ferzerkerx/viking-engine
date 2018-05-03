#ifndef __CLOUDS__H__
#define __CLOUDS__H__


#include "Perlin.h"
#include "../Framework.h"


/**
* @class Clouds
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 5:14:06 PM
*/

class Clouds {
public:
    Clouds();

    Clouds(float cover, float sharpness, int num_octavos, int width, int height);

    int texture_width();

    int texture_height();

    unsigned int texture_id();

    char *getData();

protected:
    virtual ~Clouds();

    virtual void ForceUpdate()=0;

    void CalculateAlpha();

    void GeneraGLTexture();

    unsigned char num_octaves() const;

    float sharpness() const;

    float cover() const;

    char *RGBA_text() const;

    float *map32();

    float *map() const;

    int text_width() const;

    int text_height() const;

    void set_RGBA_text(char *RGBA_text_);

    void set_map(float *map_);

    virtual void set_num_octaves(unsigned char num_octaves);

private:
    unsigned char num_octaves_;
    float sharpness_;
    float cover_;
    char *RGBA_text_;
    float map32_[32 * 32];
    float *map_;
    int text_width_;
    int text_height_;
    unsigned int texture_id_;

};

#endif