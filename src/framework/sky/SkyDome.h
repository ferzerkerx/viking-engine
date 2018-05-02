#ifndef    __SKYDOME__H__
#define __SKYDOME__H__

#include <cmath>
#include "Sky.h"


#define PI 3.1416f

#define RAD (PI/180.0F)


typedef struct {
    polar3f pol;
    vector3f pos;
    vector3f uv;
    color3f color;
} VertexDome;

/**	@class SkyDome
*	@author Fernando Montes de Oca Cespedes.
*	@version 1.0
*	@date	09/May/2007
*/
class SkyDome : public Sky {

private:
    float dphi_;
    float dtheta_;
    float r_;
    int num_vertices_;
    vector3f sun_vector_;
    polar3f sun_polar_;
    unsigned int sun_texture_;
    bool has_sun_texture_;
    VertexDome *vertex_dome_;


protected:
public:
    float dphi() const;

    float dtheta() const;

    float r() const;

    int num_vertices() const;

    const vector3f &sun_vector() const;

    const polar3f &sun_polar() const;

    unsigned int sun_texture() const;

    bool has_sun_texture() const;

    VertexDome *vertex_dome() const;

protected:
    void CreateHemiSphere();

    virtual void RenderDome(vector3f camera_position) = 0;

    void RenderSun(vector3f camera_position);

public:
    explicit SkyDome(float r);

    SkyDome(float dphi, float dtheta, float r);

    void set_sun_texture(unsigned int texture_id);

    ~SkyDome() override;
};

#endif