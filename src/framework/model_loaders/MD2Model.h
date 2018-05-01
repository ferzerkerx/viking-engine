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
#include "Model3D.h"

/**
*Enumeraciones que representan las animaciones estandard de un modelo MD2
*/
typedef enum {
    STAND, RUN, ATTACK, PAIN_A, PAIN_B, PAIN_C, JUMP,
    FLIP, SALUTE, FALLBACK, WAVE, POINTING, CROUCH_STAND,
    CROUCH_WALK, CROUCH_ATTACK, CROUCH_PAIN, CROUCH_DEATH,
    DEATH_FALLBACK, DEATH_FALLFORWARD, DEATH_FALLBACKSLOW,
    BOOM
} MD2_anim;

/**
* @class class MD2Model
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:54:03 PM
* @brief Representa un Modelo MD2
*/

class MD2Model : public Model3D {
public:
    MD2Model();

    ~MD2Model() override;

    void render() override;

    void renderWithOpenGlCommands();

    void actualiza() override;

    void addAnimation(Animacion anim);

    void setAnimation(MD2_anim anim);

    MD2_anim getCurrentAnimation();

    void nextAnimation();

    void previousAnimation();

    void setAnimationSpeed(float speed);

    void setGlCommands(int *com, int num);

    void calculaNormales();


private:
    int *m_glCommandBuffer;            //comandos de dibujo de openGL de MD2
    int num_glCommands;                    //numero de comandos de dibujo de openGL de MD2
    int currentAnim;                    // Indice la de animacion actual
    int currentFrame;                    // Frame actual de la animacion
    int nextFrame;                        // Siguiente frame de la animacion
    float m_animation_speed;            // Velocidad de la animacion
    std::vector<Animacion> pAnimations; // The list of animation
    Timer m_timer;                        //timer de la animacion para la interpolacion
    bool m_b_hasAnimation;                //nos dice si el modelo tiene animacion

    float calculaFactorInterpolacion();
};

/**
* @var MD2Model::m_glCommandBuffer
* @brief comandos de dibujo de openGL de MD2
*/

/**
* @var MD2Model::num_glCommands
* @brief numero de comandos de dibujo de openGL de MD2
*/

/**
* @var MD2Model::currentAnim
* @brief  Indice la de animacion actual
*/

/**
* @var MD2Model::currentFrame
* @brief  Frame actual de la animacion
*/

/**
* @var MD2Model::nextFrame
* @brief  Siguiente frame de la animacion
*/

/**
* @var MD2Model::m_animation_speed
* @brief  Velocidad de la animacion
*/

/**
* @var MD2Model::pAnimations
* @brief  The list of animation
*/

/**
* @var MD2Model::m_timer
* @brief timer de la animacion para la interpolacion
*/

/**
* @var MD2Model::m_b_hasAnimation
* @brief nos dice si el modelo tiene animacion
*/


#endif