/**
* @file MD2Model.h
* @brief Encabezado de la clase MD2Model
* @date Friday, November 02, 2007 12:12:57 AM
*/
#ifndef __MD2MODEL__H__
#define __MD2MODEL__H__

#include <vector>
#include "../math/vector.h"
#include "../Timer.h"
#include "Model3d.h"

typedef enum {
    STAND, RUN, ATTACK, PAIN_A, PAIN_B, PAIN_C, JUMP,
    FLIP, SALUTE, FALLBACK, WAVE, POINTING, CROUCH_STAND,
    CROUCH_WALK, CROUCH_ATTACK, CROUCH_PAIN, CROUCH_DEATH,
    DEATH_FALLBACK, DEATH_FALLFORWARD, DEATH_FALLBACKSLOW,
    BOOM
} Md2Animation;

/**
* @class class MD2Model
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:54:03 PM
*/

class Md2Model : public Model3d {
public:
    Md2Model();

    ~Md2Model() override;

    void Render() override;

    void RenderWithOpenGlCommands();

    void Update() override;

    void Add_animation(Animation animation);

    void set_current_animation(Md2Animation md2Animation);

    Md2Animation current_animation();

    void NextAnimation();

    void PreviousAnimation();

    void set_animation_speed(float speed);

    void set_GlCommands(int *com, int num);

    void updateNormalVector();


private:
    int *m_glCommand_buffer_;
    int num_glCommands_;
    int current_animation_;
    int current_frame_;
    int next_frame_;
    float animation_speed_;
    std::vector<Animation> animations_;
    Timer timer_;
    bool hasAnimation_;

    float CalculateInterpolationFactor();
};


#endif