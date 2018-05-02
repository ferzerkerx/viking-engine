#ifndef __COLOREDSKYDOME__H__
#define __COLOREDSKYDOME__H__

#include "SkyDome.h"

/**
* @class ColoredSkyDome
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 9:05:57 PM
*/

class ColoredSkyDome : public SkyDome {
public:
    explicit ColoredSkyDome(float r);

    ColoredSkyDome(float dphi, float dtheta, float r);

    void Update(vector3f camera_position) override;

protected:
    ~ColoredSkyDome() override;

    void Render(vector3f camera_position) override;

    void RenderDome(vector3f camera_position) override;

    static void ColorDome(VertexDome *vertexDome, int num_verices);
};

#endif