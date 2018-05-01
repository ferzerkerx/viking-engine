#ifndef    __SKY_H__
#define __SKY_H__

#include "../Framework.h"

/**
* @class Sky
* @author Fernando Montes de Oca Cespedes
* @date Thursday, October 18, 2007 6:58:13 PM
*/

class Sky {
public:

    virtual void Update(vector3f poscam)=0;

    virtual void Render(vector3f poscam)=0;

    virtual ~Sky() = default;;
};


#endif