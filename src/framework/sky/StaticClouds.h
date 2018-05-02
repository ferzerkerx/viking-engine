#ifndef __CLOUDSESTATICAS__H
#define __CLOUDSESTATICAS__H


#include "Clouds.h"


/**	@class StaticClouds
*	@author Fernando Montes de Oca Cespedes.
*	@version 1.0
*	@date	29/Jul/2007
*/
class StaticClouds : public Clouds {
public:
    StaticClouds();

    StaticClouds(float cover, float sharpness, int num_octavos, int width, int height);

    ~StaticClouds() override;

    void ForceUpdate() override;

private:
    void CreateTexture() override;

};

#endif

