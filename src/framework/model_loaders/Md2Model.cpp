/**
* @file MD2Model.cpp
* @brief Implementacin de la clase MD2Model
* @date Friday, November 02, 2007 12:11:58 AM
*/
#include "Md2Model.h"
#include <GL/glut.h>
#include <cstring>

/**
* @brief Constrtuctor que recibe la ruta y la textura usada por el modelo MD2
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:39:31 PM
*/
Md2Model::Md2Model() {
    hasAnimation_ = false;
    m_glCommand_buffer_ = nullptr;
    num_glCommands_ = 0;
    current_animation_ = STAND;
    current_frame_ = 0;
    next_frame_ = 0;
    animation_speed_ = 1.0F;
}


/**
* @brief Destructor que libera las animaciones y los comandos utilizados
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:41:19 PM
*/
Md2Model::~Md2Model() {
    while (!animations_.empty()) {
        animations_.pop_back();
    }
    delete m_glCommand_buffer_;
}


/**
* @brief Dibuja el modelo en el frame actual
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:41:44 PM
*/
void Md2Model::Render() {
    if (objects.empty()) return;

    float t = 0.0F;
    int i = 0;
    int j = 0;
    int index = 0;
    int index2 = 0;

    if (hasAnimation_) {
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        if (next_frame_ == 0) { next_frame_ = animations_[current_animation_].initial_frame; }
    }

    Object3D *frame = &objects[current_frame_];
    Object3D *next_frame = &objects[next_frame_];
    Object3D *first_frame = &objects[0];

    if (hasAnimation_) { t = CalculateInterpolationFactor(); }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!materials_.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(materials_[0].texture_id));
    }

    glBegin(GL_TRIANGLES);
    for (j = 0; j < frame->num_faces; j++) {
        for (i = 0; i < 3; i++) {
            index = first_frame->faces[j].vert_index[i];
            index2 = first_frame->faces[j].st_index[i];

            if (frame->normal) {
                glNormal3f(frame->normal[index].x, frame->normal[index].y, frame->normal[index].z);
            }

            if (first_frame->texture_st && !materials_.empty()) {
                glTexCoord2f(first_frame->texture_st[index2].s, first_frame->texture_st[index2].t);
            }

            if (hasAnimation_) {
                glVertex3f(frame->vertex[index].x + t * (next_frame->vertex[index].x - frame->vertex[index].x),
                           frame->vertex[index].y + t * (next_frame->vertex[index].y - frame->vertex[index].y),
                           frame->vertex[index].z + t * (next_frame->vertex[index].z - frame->vertex[index].z));
            } else {
                glVertex3f(frame->vertex[index].x, frame->vertex[index].y, frame->vertex[index].z);
            }
        }
    }

    glEnd();

    if (!materials_.empty()) {
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_CULL_FACE);
}

void Md2Model::Update() {
    // nothing to do here
}

void Md2Model::Add_animation(Animation animation) {
    animations_.push_back(animation);
    hasAnimation_ = true;
}


void Md2Model::set_current_animation(Md2Animation md2Animation) {
    current_animation_ = md2Animation;
    if (current_animation_ < STAND) {
        current_animation_ = static_cast<int>(animations_.size() - 1);
    } else if (current_animation_ >= animations_.size()) {
        current_animation_ = STAND;
    }

    current_frame_ = animations_[current_animation_].initial_frame;
    next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
}

/**
* @brief Regresamos la animacion actual
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:44:19 PM
* @retval MD2_anim La animacion actual
*/
Md2Animation Md2Model::current_animation() {
    return *((Md2Animation *) &current_animation_);
}

/**
* @brief Coloca la siguiente animacion disponible
* regresa a la primer animacion si es que no hay otra
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:44:47 PM
*/
void Md2Model::NextAnimation() {
    current_animation_++;
    set_current_animation(*((Md2Animation *) &current_animation_));
}

/**
* @brief Coloca la animacion anterior disponible
* regresa a la ultima animacion si es que no hay otra
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:44:47 PM
*/
void Md2Model::PreviousAnimation() {
    current_animation_--;
    set_current_animation(*((Md2Animation *) &current_animation_));
}


/**
* @brief Metodo que calcula el factor de interpolacion entre
* frames de acuerdo al tiempo y a la velocidad de la animacion
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:45:50 PM
* @retval float Valor que va de 0.0F a 1.0F representando el factor
* de interpolacion de un frame a otro
*/
float Md2Model::CalculateInterpolationFactor() {
    float t = timer_.getMilliseconds() / (1000.0F / animation_speed_);

    if (timer_.getMilliseconds() >= (1000.0F / animation_speed_)) {
        current_frame_ = next_frame_;
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        timer_.reset();
    }
    if (t > 1.0F) { t = 1.0F; }
    return t;
}

/**
* @brief Metodo que cambia la velocidad de la animacion
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:47:01 PM
* @param speed Velocidad nueva
*/
void Md2Model::set_animation_speed(float speed) {
    animation_speed_ = speed;
    if (animation_speed_ < 0.0F) { animation_speed_ = 0.0F; }
}


/**
* @brief Metodo que llena los comandos de OpenGL incluidos en el
* MD2
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:47:43 PM
* @param com Apuntador a los comandos de OpenGL
* @param num Numero de comandos
*/
void Md2Model::set_GlCommands(int *com, int num) {
    if (num_glCommands_ > 0) { delete m_glCommand_buffer_; }
    m_glCommand_buffer_ = new int[num];
    memcpy(m_glCommand_buffer_, com, static_cast<size_t>(num * 4));
    num_glCommands_ = num;
}

/**
* @brief Dibuja el modelo usando comandos de OpenGL tambien lo anima
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:48:40 PM
*/
void Md2Model::RenderWithOpenGlCommands() {

    if (objects.empty() || !m_glCommand_buffer_) return;

    int *ptricmds = m_glCommand_buffer_;
    float t = 0.0F;
    if (hasAnimation_) {
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        if (next_frame_ == 0) { next_frame_ = animations_[current_animation_].initial_frame; }
    }

    Object3D *frame = &objects[current_frame_];
    Object3D *next_frame = &objects[next_frame_];

    if (hasAnimation_) { t = CalculateInterpolationFactor(); }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!materials_.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(materials_[0].texture_id));
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
            if (!materials_.empty()) {
                glTexCoord2f(((float *) ptricmds)[0], 1.0F - ((float *) ptricmds)[1]);
            }

            if (frame->normal) {
                glNormal3f(frame->normal[ptricmds[2]].x, frame->normal[ptricmds[2]].y,
                           frame->normal[ptricmds[2]].z);
            }

            if (hasAnimation_) {
                glVertex3f(frame->vertex[ptricmds[2]].x +
                           t * (next_frame->vertex[ptricmds[2]].x - frame->vertex[ptricmds[2]].x),
                           frame->vertex[ptricmds[2]].y +
                           t * (next_frame->vertex[ptricmds[2]].y - frame->vertex[ptricmds[2]].y),
                           frame->vertex[ptricmds[2]].z +
                           t * (next_frame->vertex[ptricmds[2]].z - frame->vertex[ptricmds[2]].z));
            } else {
                glVertex3f(frame->vertex[ptricmds[2]].x, frame->vertex[ptricmds[2]].y,
                           frame->vertex[ptricmds[2]].z);
            }
        }

        glEnd();
    }
    if (!materials_.empty()) {
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_CULL_FACE);
}

/**
* @brief Metodo que calcula las normales para cada cara de cada 
* objeto del modelo
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:50:12 PM
*/
void Md2Model::updateNormalVector() {
    vector3f vVector1;
    vector3f vVector2;
    vector3f vNormal;
    vector3f vPoly[3];

    if (objects.empty()) return;

    Object3D *first = &(objects[0]);

    for (auto &m_object : objects) {

        Object3D *obj = &m_object;
        auto *normales = new vector3f[obj->num_faces];
        auto *pTempNormals = new vector3f[obj->num_faces];
        obj->normal = new vector3f[obj->num_verts];

        for (int i = 0; i < first->num_faces; i++) {

            vPoly[0] = obj->vertex[first->faces[i].vert_index[0]];
            vPoly[1] = obj->vertex[first->faces[i].vert_index[1]];
            vPoly[2] = obj->vertex[first->faces[i].vert_index[2]];

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


            obj->normal[i] = vSum / float(-shared);
            obj->normal[i] = Normalizar(obj->normal[i]) * (-1.0F);

            vSum = vZero;
            shared = 0;
        }

        delete[] pTempNormals;
        delete[] normales;
    }
}