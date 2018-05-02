#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Sky.h"

/**
* @class SkyBox
* @author Fernando Montes de Oca Cespedes
* @date Saturday, October 20, 2007 1:28:06 PM
*/

class SkyBox : public Sky {
private:
    unsigned int faces_[6];
    float height_;
    float large_;
    float width_;

protected:
    enum Faces {
        FRONT, BACK, LEFT, RIGHT, UP, DOWN
    };

    void Render(vector3f camera_position) override;

public:
    SkyBox(float height, float large, float width, unsigned int *faces);

    SkyBox(float height, float large, float width, unsigned int front,
           unsigned int back, unsigned int left, unsigned int right, unsigned int up, unsigned int down);

    void Update(vector3f camera_position) override;

    float height();

    float large();

    float width();

    ~SkyBox() override;

};

#endif