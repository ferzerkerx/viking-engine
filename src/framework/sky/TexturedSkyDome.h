#ifndef __TEXTUREDSKYDOME_H__
#define __TEXTUREDSKYDOME_H__


#include "SkyDome.h"

/**	@class TexturedSkyDome
*	@author Fernando Montes de Oca Cespedes.
*	@version 1.0
*	@date	09/May/2007
*/
class TexturedSkyDome : public SkyDome {
protected:

    unsigned int texture_id_;
    float h_tile_;
    float v_tile_;
    void Render(vector3f camera_position) override;

    void RenderDome(vector3f camera_position) override;

    void CalculateUV();


public:
    TexturedSkyDome(float r, unsigned int texture_id);

    TexturedSkyDome(float dphi, float dtheta, float r, unsigned int texture_id);

    void Update(vector3f camera_position) override;

    void set_texture_id(unsigned int texture_id);

    ~TexturedSkyDome() override;

    void AdjustUVRanges() const;
};


#endif

