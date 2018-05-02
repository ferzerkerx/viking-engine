#include "Md2Model.h"
#include <GL/glut.h>
#include <cstring>

Md2Model::Md2Model() {
    hasAnimation_ = false;
    glCommand_buffer_ = nullptr;
    num_glCommands_ = 0;
    current_animation_ = STAND;
    current_frame_ = 0;
    next_frame_ = 0;
    animation_speed_ = 1.0F;
}

Md2Model::~Md2Model() {
    while (!animations_.empty()) {
        animations_.pop_back();
    }
    delete glCommand_buffer_;
}

void Md2Model::Render() {
    if (objects.empty()) {
        return;
    }

    float interpolation_factor = 0.0F;
    int i = 0;
    int faces_number = 0;
    int vertex_index = 0;
    int textture_index = 0;

    if (hasAnimation_) {
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        if (next_frame_ == 0) {
            next_frame_ = animations_[current_animation_].initial_frame;
        }
    }

    Object3D *frame = &objects[current_frame_];
    Object3D *next_frame = &objects[next_frame_];
    Object3D *first_frame = &objects[0];

    if (hasAnimation_) { interpolation_factor = CalculateInterpolationFactor(); }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!materials_.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(materials_[0].texture_id));
    }

    glBegin(GL_TRIANGLES);
    for (faces_number = 0; faces_number < frame->num_faces; faces_number++) {
        for (i = 0; i < 3; i++) {
            vertex_index = first_frame->faces[faces_number].vert_index[i];
            textture_index = first_frame->faces[faces_number].st_index[i];

            if (frame->normal) {
                glNormal3f(frame->normal[vertex_index].x, frame->normal[vertex_index].y, frame->normal[vertex_index].z);
            }

            if (first_frame->texture_st && !materials_.empty()) {
                glTexCoord2f(first_frame->texture_st[textture_index].s, first_frame->texture_st[textture_index].t);
            }

            if (hasAnimation_) {
                glVertex3f(frame->vertex[vertex_index].x +
                           interpolation_factor * (next_frame->vertex[vertex_index].x - frame->vertex[vertex_index].x),
                           frame->vertex[vertex_index].y +
                           interpolation_factor * (next_frame->vertex[vertex_index].y - frame->vertex[vertex_index].y),
                           frame->vertex[vertex_index].z +
                           interpolation_factor * (next_frame->vertex[vertex_index].z - frame->vertex[vertex_index].z));
            } else {
                glVertex3f(frame->vertex[vertex_index].x, frame->vertex[vertex_index].y, frame->vertex[vertex_index].z);
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

Md2Animation Md2Model::current_animation() {
    return *((Md2Animation *) &current_animation_);
}

void Md2Model::NextAnimation() {
    current_animation_++;
    set_current_animation(*((Md2Animation *) &current_animation_));
}

void Md2Model::PreviousAnimation() {
    current_animation_--;
    set_current_animation(*((Md2Animation *) &current_animation_));
}


float Md2Model::CalculateInterpolationFactor() {
    float factor = timer_.Milliseconds() / (1000.0F / animation_speed_);

    if (timer_.Milliseconds() >= (1000.0F / animation_speed_)) {
        current_frame_ = next_frame_;
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        timer_.Reset();
    }
    if (factor > 1.0F) {
        factor = 1.0F;
    }
    return factor;
}

void Md2Model::set_animation_speed(float speed) {
    animation_speed_ = speed;
    if (animation_speed_ < 0.0F) { animation_speed_ = 0.0F; }
}


void Md2Model::set_GlCommands(int *com, int num) {
    if (num_glCommands_ > 0) { delete glCommand_buffer_; }
    glCommand_buffer_ = new int[num];
    memcpy(glCommand_buffer_, com, static_cast<size_t>(num * 4));
    num_glCommands_ = num;
}


void Md2Model::RenderWithOpenGlCommands() {

    if (objects.empty() || !glCommand_buffer_) return;

    int *command_buffer_ = glCommand_buffer_;
    float interpolation_factor = 0.0F;
    if (hasAnimation_) {
        next_frame_ = (current_frame_ + 1) % animations_[current_animation_].final_frame;
        if (next_frame_ == 0) { next_frame_ = animations_[current_animation_].initial_frame; }
    }

    Object3D *frame = &objects[current_frame_];
    Object3D *next_frame = &objects[next_frame_];

    if (hasAnimation_) {
        interpolation_factor = CalculateInterpolationFactor();
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    if (!materials_.empty()) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(materials_[0].texture_id));
    }

    while (int i = *(command_buffer_++)) {
        if (i < 0) {
            glBegin(GL_TRIANGLE_FAN);
            i = -i;
        } else { glBegin(GL_TRIANGLE_STRIP); }


        int s_index = 0;
        int t_index = 1;
        int vertex_index = 2;
        for (; i > 0; i--, command_buffer_ += 3) {
            if (!materials_.empty()) {
                glTexCoord2f(((float *) command_buffer_)[s_index], 1.0F - ((float *) command_buffer_)[t_index]);
            }

            if (frame->normal) {
                glNormal3f(frame->normal[command_buffer_[vertex_index]].x,
                           frame->normal[command_buffer_[vertex_index]].y,
                           frame->normal[command_buffer_[vertex_index]].z);
            }

            if (hasAnimation_) {
                float x = frame->vertex[command_buffer_[vertex_index]].x +
                          interpolation_factor * (next_frame->vertex[command_buffer_[vertex_index]].x -
                               frame->vertex[command_buffer_[vertex_index]].x);
                float y = frame->vertex[command_buffer_[vertex_index]].y +
                          interpolation_factor * (next_frame->vertex[command_buffer_[vertex_index]].y -
                               frame->vertex[command_buffer_[vertex_index]].y);
                float z = frame->vertex[command_buffer_[vertex_index]].z +
                          interpolation_factor * (next_frame->vertex[command_buffer_[vertex_index]].z -
                               frame->vertex[command_buffer_[vertex_index]].z);
                glVertex3f(x, y, z);
            } else {
                glVertex3f(frame->vertex[command_buffer_[vertex_index]].x,
                           frame->vertex[command_buffer_[vertex_index]].y,
                           frame->vertex[command_buffer_[vertex_index]].z);
            }
        }

        glEnd();
    }
    if (!materials_.empty()) {
        glDisable(GL_TEXTURE_2D);
    }
    glDisable(GL_CULL_FACE);
}


void Md2Model::updateNormalVector() {
    vector3f vector;
    vector3f auxiliar;
    vector3f normal;
    vector3f polygon[3];

    if (objects.empty()) return;

    Object3D *first = &(objects[0]);

    for (auto &object : objects) {
        Object3D *obj = &object;
        auto *normals = new vector3f[obj->num_faces];
        auto *temp_normals = new vector3f[obj->num_faces];
        obj->normal = new vector3f[obj->num_verts];

        for (int i = 0; i < first->num_faces; i++) {

            polygon[0] = obj->vertex[first->faces[i].vert_index[0]];
            polygon[1] = obj->vertex[first->faces[i].vert_index[1]];
            polygon[2] = obj->vertex[first->faces[i].vert_index[2]];

            vector = polygon[0] - polygon[2];
            auxiliar = polygon[2] - polygon[1];

            normal = Cross(vector, auxiliar);
            temp_normals[i] = normal;
            normal = Normalize(normal);

            normals[i] = normal;
        }


        vector3f sumVector(0.0F, 0.0F, 0.0F);
        vector3f zero = sumVector;
        int shared = 0;

        for (int i = 0; i < obj->num_verts; i++) {
            for (int j = 0; j < first->num_faces; j++) {

                if (first->faces[j].vert_index[0] == i ||
                    first->faces[j].vert_index[1] == i ||
                    first->faces[j].vert_index[2] == i) {

                    sumVector = sumVector + temp_normals[j];
                    shared++;
                }
            }


            obj->normal[i] = sumVector / float(-shared);
            obj->normal[i] = Normalize(obj->normal[i]) * (-1.0F);

            sumVector = zero;
            shared = 0;
        }

        delete[] temp_normals;
        delete[] normals;
    }
}