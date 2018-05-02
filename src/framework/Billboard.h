#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "math/vector.h"

/**	@class Billboard
*	@author Luis Daniel Rodrguez P.
*	@version 2.0
*	@date	09/Jun/2007
*/
class Billboard {

public:
    static void BBFalseSphere();

    static void BBFalseCylinder();

    static void BBCylinder(vector3f camera_position, float obj_pos_x, float obj_pos_y, float obj_pos_z);

    static void BBSphere(vector3f camera_position, float obj_pos_x, float obj_pos_y, float obj_pos_z);

    static void BBFin();

    static vector3f ObjectCameraPositionBasedOnObject(const vector3f &camera_position, float obj_pos_x, float obj_pos_z);

    static vector3f RotateBasedOnCameraPosition(const vector3f &camera_position, float obj_pos_x, float obj_pos_z);
};

#endif
