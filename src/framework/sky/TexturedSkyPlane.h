#ifndef __TEXTUREDSKYPLANE__H__
#define __TEXTUREDSKYPLANE__H__

#include "Sky.h"


#define SQR(x)((x)*(x))


/**
* @class TexturedSkyPlane
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 9:36:24 AM
*/
class TexturedSkyPlane : public Sky {
private:

    typedef struct {
        vector3f pos;
        float u{};
        float v{};
    } vertx;

    float m_h_tile;
    float m_v_tile;
    unsigned int m_texture;
    unsigned int num_vertex_;
    unsigned int num_indexes_;
    unsigned int divisions_;
    float atmosphere_radius_;
    unsigned int *indexes_;
    vertx *vertex_;
    float planet_radius_;
    float roty_factor_;
    float wind_factor_x_;
    float wind_factor_y_;
    unsigned int rot_texture_in_milli_;
    float texture_delta_x;
    float delta_texture_y_;
    Timer timer_;

    void Render(vector3f poscam) override;

    void GenerateSkyPlane();


public:
    TexturedSkyPlane(int divisions, float planet_radius, float atmosphere_radius, float h_tile, float v_tile,
                     unsigned int texture);

    void Update(vector3f camera_position) override;

    void set_wind_factor_x(float wfx);

    void set_wind_factor_y(float wfy);

    void set_wind_factor_XY(float wfx, float wfy);

    ~TexturedSkyPlane() override;

};

#endif