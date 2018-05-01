/**
* @file MD2Model.cpp
* @brief Implementacin de la clase MD2Model
* @date Friday, November 02, 2007 12:11:58 AM
*/
#include "MD2Model.h"
#include "MD2Loader.h"
#include <GL/glut.h>
#include <cstring>

/**
* @brief Constrtuctor que recibe la ruta y la textura usada por el modelo MD2
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:39:31 PM
*/
MD2Model::MD2Model() {
    m_b_hasAnimation = false;
    m_glCommandBuffer = nullptr;
    num_glCommands = 0;
    currentAnim = STAND;
    currentFrame = 0;
    nextFrame = 0;
    m_animation_speed = 1.0F;
}


/**
* @brief Destructor que libera las animaciones y los comandos utilizados
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:41:19 PM
*/
MD2Model::~MD2Model() {
    while (!pAnimations.empty()) {
        pAnimations.pop_back();
    }
    delete m_glCommandBuffer;
}


/**
* @brief Dibuja el modelo en el frame actual
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:41:44 PM
*/
void MD2Model::render() {
    if (m_objects.empty()) return;

    float t = 0.0F;
    int i = 0;
    int j = 0;
    int index = 0;
    int index2 = 0;

    if (m_b_hasAnimation) {
        nextFrame = (currentFrame + 1) % pAnimations[currentAnim].frame_final;
        if (nextFrame == 0) { nextFrame = pAnimations[currentAnim].frame_inicial; }
    }

    Object3D *frame = &m_objects[currentFrame];
    Object3D *next_frame = &m_objects[nextFrame];
    Object3D *first_frame = &m_objects[0];

    if (m_b_hasAnimation) { t = calculaFactorInterpolacion(); }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!m_materials.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(m_materials[0].texture_id));
    }

    glBegin(GL_TRIANGLES);
    for (j = 0; j < frame->num_faces; j++) {
        for (i = 0; i < 3; i++) {
            index = first_frame->faces[j].vert_index[i];
            index2 = first_frame->faces[j].st_index[i];

            if (frame->normales) {
                glNormal3f(frame->normales[index].x, frame->normales[index].y, frame->normales[index].z);
            }

            if (first_frame->text_st && !m_materials.empty()) {
                glTexCoord2f(first_frame->text_st[index2].s, first_frame->text_st[index2].t);
            }

            if (m_b_hasAnimation) {
                glVertex3f(frame->vertices[index].x + t * (next_frame->vertices[index].x - frame->vertices[index].x),
                           frame->vertices[index].y + t * (next_frame->vertices[index].y - frame->vertices[index].y),
                           frame->vertices[index].z + t * (next_frame->vertices[index].z - frame->vertices[index].z));
            } else {
                glVertex3f(frame->vertices[index].x, frame->vertices[index].y, frame->vertices[index].z);
            }
        }
    }

    glEnd();

    if (!m_materials.empty()) {
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_CULL_FACE);
}

/**
* @brief Metodo que actualiza el modelo
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:42:57 PM
*/
void MD2Model::actualiza() {
    // nothing to do here
}

/**
* @brief Agrega una animacion al modelo actual
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:43:20 PM
* @param anim Animacion nueva
*/
void MD2Model::addAnimation(Animacion anim) {
    pAnimations.push_back(anim);
    m_b_hasAnimation = true;
}

/**
* @brief Metodo que cambia la animacion actual
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:43:46 PM
* @param anim Animacion deseada
*/
void MD2Model::setAnimation(MD2_anim anim) {
    currentAnim = anim;
    if (currentAnim < STAND) {
        currentAnim = static_cast<int>(pAnimations.size() - 1);
    } else if (currentAnim >= pAnimations.size()) {
        currentAnim = STAND;
    }

    currentFrame = pAnimations[currentAnim].frame_inicial;
    nextFrame = (currentFrame + 1) % pAnimations[currentAnim].frame_final;
}

/**
* @brief Regresamos la animacion actual
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:44:19 PM
* @retval MD2_anim La animacion actual
*/
MD2_anim MD2Model::getCurrentAnimation() {
    return *((MD2_anim *) &currentAnim);
}

/**
* @brief Coloca la siguiente animacion disponible
* regresa a la primer animacion si es que no hay otra
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:44:47 PM
*/
void MD2Model::nextAnimation() {
    currentAnim++;
    setAnimation(*((MD2_anim *) &currentAnim));
}

/**
* @brief Coloca la animacion anterior disponible
* regresa a la ultima animacion si es que no hay otra
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:44:47 PM
*/
void MD2Model::previousAnimation() {
    currentAnim--;
    setAnimation(*((MD2_anim *) &currentAnim));
}


/**
* @brief Metodo que calcula el factor de interpolacion entre
* frames de acuerdo al tiempo y a la velocidad de la animacion
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:45:50 PM
* @retval float Valor que va de 0.0F a 1.0F representando el factor
* de interpolacion de un frame a otro
*/
float MD2Model::calculaFactorInterpolacion() {
    float t = m_timer.getMilliseconds() / (1000.0F / m_animation_speed);

    if (m_timer.getMilliseconds() >= (1000.0F / m_animation_speed)) {
        currentFrame = nextFrame;
        nextFrame = (currentFrame + 1) % pAnimations[currentAnim].frame_final;
        m_timer.reset();
    }
    if (t > 1.0F) { t = 1.0F; }
    return t;
}

/**
* @brief Metodo que cambia la velocidad de la animacion
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:47:01 PM
* @param speed Velocidad nueva
*/
void MD2Model::setAnimationSpeed(float speed) {
    m_animation_speed = speed;
    if (m_animation_speed < 0.0F) { m_animation_speed = 0.0F; }
}


/**
* @brief Metodo que llena los comandos de OpenGL incluidos en el
* MD2
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:47:43 PM
* @param com Apuntador a los comandos de OpenGL
* @param num Numero de comandos
*/
void MD2Model::setGlCommands(int *com, int num) {
    if (num_glCommands > 0) { delete m_glCommandBuffer; }
    m_glCommandBuffer = new int[num];
    memcpy(m_glCommandBuffer, com, static_cast<size_t>(num * 4));
    num_glCommands = num;
}

/**
* @brief Dibuja el modelo usando comandos de OpenGL tambien lo anima
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:48:40 PM
*/
void MD2Model::renderWithOpenGlCommands() {

    if (m_objects.empty() || !m_glCommandBuffer) return;

    int *ptricmds = m_glCommandBuffer;
    float t = 0.0F;
    if (m_b_hasAnimation) {
        nextFrame = (currentFrame + 1) % pAnimations[currentAnim].frame_final;
        if (nextFrame == 0) { nextFrame = pAnimations[currentAnim].frame_inicial; }
    }

    Object3D *frame = &m_objects[currentFrame];
    Object3D *next_frame = &m_objects[nextFrame];

    if (m_b_hasAnimation) { t = calculaFactorInterpolacion(); }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!m_materials.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(m_materials[0].texture_id));
    }

    while (int i = *(ptricmds++)) {
        if (i < 0) {
            glBegin(GL_TRIANGLE_FAN);
            i = -i;
        } else { glBegin(GL_TRIANGLE_STRIP); }


        for (; i > 0; i--, ptricmds += 3) {
            // ptricmds[0] : textura  s
            // ptricmds[1] : textura t
            // ptricmds[2] : indice del vertice
            if (!m_materials.empty()) {
                glTexCoord2f(((float *) ptricmds)[0], 1.0F - ((float *) ptricmds)[1]);
            }

            if (frame->normales) {
                glNormal3f(frame->normales[ptricmds[2]].x, frame->normales[ptricmds[2]].y,
                           frame->normales[ptricmds[2]].z);
            }

            if (m_b_hasAnimation) {
                glVertex3f(frame->vertices[ptricmds[2]].x +
                           t * (next_frame->vertices[ptricmds[2]].x - frame->vertices[ptricmds[2]].x),
                           frame->vertices[ptricmds[2]].y +
                           t * (next_frame->vertices[ptricmds[2]].y - frame->vertices[ptricmds[2]].y),
                           frame->vertices[ptricmds[2]].z +
                           t * (next_frame->vertices[ptricmds[2]].z - frame->vertices[ptricmds[2]].z));
            } else {
                glVertex3f(frame->vertices[ptricmds[2]].x, frame->vertices[ptricmds[2]].y,
                           frame->vertices[ptricmds[2]].z);
            }
        }

        glEnd();
    }
    if (!m_materials.empty()) {
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_CULL_FACE);
}

/**
* @brief Metodo que calcula las normales para cada cara de cada 
* objeto del modelo
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:50:12 PM
*/
void MD2Model::calculaNormales() {
    vector3f vVector1;
    vector3f vVector2;
    vector3f vNormal;
    vector3f vPoly[3];

    if (m_objects.empty()) return;

    Object3D *first = &(m_objects[0]);

    for (auto &m_object : m_objects) {

        Object3D *obj = &m_object;
        auto *normales = new vector3f[obj->num_faces];
        auto *pTempNormals = new vector3f[obj->num_faces];
        obj->normales = new vector3f[obj->num_verts];

        for (int i = 0; i < first->num_faces; i++) {

            vPoly[0] = obj->vertices[first->faces[i].vert_index[0]];
            vPoly[1] = obj->vertices[first->faces[i].vert_index[1]];
            vPoly[2] = obj->vertices[first->faces[i].vert_index[2]];

            vVector1 = vPoly[0] - vPoly[2];
            vVector2 = vPoly[2] - vPoly[1];

            vNormal = Cruzado(vVector1, vVector2);
            pTempNormals[i] = vNormal;
            vNormal = Normalizar(vNormal);

            normales[i] = vNormal;
        }


        vector3f vSum(0.0F, 0.0F, 0.0F);
        vector3f vZero = vSum;
        int shared = 0;

        for (int i = 0; i < obj->num_verts; i++) {
            for (int j = 0; j < first->num_faces; j++) {

                if (first->faces[j].vert_index[0] == i ||
                    first->faces[j].vert_index[1] == i ||
                    first->faces[j].vert_index[2] == i) {

                    vSum = vSum + pTempNormals[j];
                    shared++;
                }
            }


            obj->normales[i] = vSum / float(-shared);
            obj->normales[i] = Normalizar(obj->normales[i]) * (-1.0F);

            vSum = vZero;
            shared = 0;
        }

        delete[] pTempNormals;
        delete[] normales;
    }
}