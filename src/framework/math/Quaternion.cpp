#include <cmath>
#include <cstdio>
#include <cmath>
#include "Quaternion.h"
#include "vector.h"


Quaternion::Quaternion() {
    w_ = 1.0F;
    x_ = 0.0F;
    y_ = 0.0F;
    z_ = 0.0F;
}

Quaternion::Quaternion(float w, float x, float y, float z) : w_(w), x_(x), y_(y), z_(z) {

}

Quaternion::Quaternion(float theta, vector3f *axis) {
    /**
    * q = cos(theta/2) + Axis sin(theta/2)
    **/
    if (theta > 360.0F) { theta = 360.0F; }
    if (theta < 0.0F) { theta = 0.0F; }
    w_ = std::cos(theta / 2.0F);
    x_ = axis->x * std::sin(theta / 2.0F);
    y_ = axis->y * std::sin(theta / 2.0F);
    z_ = axis->z * std::sin(theta / 2.0F);
}

Quaternion::Quaternion(float *matrix, int size) {

    if (!matrix || ((size != 3) && (size != 4))) return;

    float *m = matrix;
    float m4x4[16] = {0};

    //homogenize matrix
    if (size == 3) {
        m4x4[0] = m[0];
        m4x4[1] = m[1];
        m4x4[2] = m[2];
        m4x4[4] = m[3];
        m4x4[5] = m[4];
        m4x4[6] = m[5];
        m4x4[8] = m[6];
        m4x4[9] = m[7];
        m4x4[10] = m[8];

        m4x4[15] = 1;

        m = &m4x4[0];
    }

    float trace = m[0] + m[5] + m[10]; //t00 + t11 + t22

    float scale;

    if (trace > 0.00000001) {/* |w_|> 1/2 **/

        /*
        * w_ = sqrt(trace + 1.0F) * (1/2)
        * scale = 4w
        * x_ = (t12 - t21)/4w
        * y_ = (t20 - t02)/4w
        * z_ = (t01 - t10)/4w
        */
        w_ = std::sqrt(trace + 1.0F) * 0.5F;
        scale = 4 * w_;
        x_ = (m[6] - m[9]) / scale;
        y_ = (m[8] - m[2]) / scale;
        z_ = (m[1] - m[4]) / scale;
    } else { /* |w_|<= 1/2 **/

        bool first_element_diagonal_is_bigger = m[0] > m[5] && m[0] > m[10];
        if (first_element_diagonal_is_bigger) {

            /*
            * x_ = sqrt(t00 - t11 - t22 + 1 ) * (1/2)
            * scale = 4x
            * w_ = (t12 - t21)/4x
            * y_ = (t01 + t10)/4x
            * z_ = (t02 + t20)/4x
            */
            x_ = std::sqrt(1.0F + m[0] - m[5] - m[10]) * 0.5F;
            scale = 4 * x_;
            w_ = (m[6] - m[9]) / scale;
            y_ = (m[1] + m[4]) / scale;
            z_ = (m[2] + m[8]) / scale;
        }
        else if (m[5] > m[10]) {
            /*
            * y_ = sqrt(t11 - t00 - t22 + 1 ) * (1/2)
            * scale = 4y
            * x_ = (t01 + t10)/4y
            * z_ = (t12 + t21)/4y
            * w_ = (t20 - t02)/4y
            */
            y_ = std::sqrt(1.0F + m[5] - m[0] - m[10]) * 0.5F;
            scale = 4 * y_;
            x_ = (m[1] + m[4]) / scale;
            z_ = (m[6] + m[9]) / scale;
            w_ = (m[8] - m[2]) / scale;

        }
        else {

            /*
            * z_ = sqrt(t22 - t00 - t11 + 1 ) * (1/2)
            * scale = 4z
            * x_ = (t02 + t20)/4z
            * y_ = (t12 + t21)/4z
            * w_ = (t01 - t10)/4z
            */
            z_ = std::sqrt(1.0F + m[10] - m[0] - m[5]) * 0.5F;
            scale = 4 * z_;
            x_ = (m[2] + m[8]) / scale;
            y_ = (m[6] + m[9]) / scale;
            w_ = (m[1] - m[4]) / scale;
        }
    }
}


bool Quaternion::operator==(const Quaternion &q) {
    return w_ == q.w_ && x_ == q.x_ && y_ == q.y_ && z_ == q.z_;
}

Quaternion Quaternion::operator+(const Quaternion &q) {
    return {w_ + q.w_, x_ + q.x_, y_ + q.y_, z_ + q.z_};
}

Quaternion Quaternion::operator-(const Quaternion &q) {
    return {w_ - q.w_, x_ - q.x_, y_ - q.y_, z_ - q.z_};
}

Quaternion Quaternion::operator*(const Quaternion &q) {
    return {w_ * q.w_ - x_ * q.x_ - y_ * q.y_ - z_ * q.z_,
          w_ * q.x_ + x_ * q.w_ + y_ * q.z_ - z_ * q.y_,
          w_ * q.y_ - x_ * q.z_ + y_ * q.w_ + z_ * q.x_,
          w_ * q.z_ + x_ * q.y_ - y_ * q.x_ + z_ * q.w_};
}


Quaternion Quaternion::operator*(float f) {
    return {w_ * f, x_ * f, y_ * f, z_ * f};
}


vector3f Quaternion::operator*(vector3f vector) {
    return RotateVector(vector);
}


Quaternion Quaternion::operator/(float f) {
    if (f == 0.0F) {
        return {0.0F, 0.0F, 0.0F, 0.0F};
    }
    return {w_ / f, x_ / f, y_ / f, z_ / f};
}


Quaternion &Quaternion::operator+=(const Quaternion &q) {
    w_ += q.w_;
    x_ += q.x_;
    y_ += q.y_;
    z_ += q.z_;
    return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &q) {
    w_ -= q.w_;
    x_ -= q.x_;
    y_ -= q.y_;
    z_ -= q.z_;
    return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &q) {
    w_ = w_ * q.w_ - x_ * q.x_ - y_ * q.y_ - z_ * q.z_;
    x_ = w_ * q.x_ + x_ * q.w_ + y_ * q.z_ - z_ * q.y_;
    y_ = w_ * q.y_ - x_ * q.z_ + y_ * q.w_ + z_ * q.x_;
    z_ = w_ * q.z_ + x_ * q.y_ - y_ * q.x_ + z_ * q.w_;
    return *this;
}

Quaternion &Quaternion::operator*=(float f) {
    w_ *= f;
    x_ *= f;
    y_ *= f;
    z_ *= f;
    return *this;
}

Quaternion &Quaternion::operator/=(float f) {
    if (f != 0.0F) {
        w_ /= f;
        x_ /= f;
        y_ /= f;
        z_ /= f;
    }
    return *this;
}

vector3f Quaternion::RotateVector(vector3f vector) {
    /**
    * - Protado = qPq^1 = P * ( w*w - (1 -w*w)) +  (2w * (q_xyz x P) + (2 * (q_xyz . P) * q_xyz)
    * - Protado = qPq^1 = P * ( 2w*w - 1) +  (2w * (q_xyz x P) + (2 * (q_xyz . P) * q_xyz)
    **/

    vector3f q_xyz(x_, y_, z_);

    //(P * ( 2w*w_ - 1))
    vector3f t1 = vector * (2.0F * w_ * w_ - 1.0F);

    // (2w * (q_xyz x_ P)
    vector3f t2 = Cross(q_xyz, vector) * 2.0F * w_;

    //(2 * (q_xyz . P) * q_xyz)
    vector3f t3 = q_xyz * 2.0F * Point(q_xyz, vector);

    return t1 + t2 + t3;
}

Quaternion Quaternion::Normalize() {
    float mag = getMagnitud();
    return {w_ / mag, x_ / mag, y_ / mag, z_ / mag};
}

Quaternion Quaternion::Conjugate() {
    return {w_, -x_, -y_, -z_};
}


Quaternion Quaternion::Inverse() {
    float sqr = w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
    return {w_ / sqr, -x_ / sqr, -y_ / sqr, -z_ / sqr};
}


Quaternion Quaternion::SLERP(const Quaternion &q1, const Quaternion &q2, float t) {
    /**
    * qm = (q1* sin(theta * (1 - t)) + q2 * sin(t * theta))/sin(theta)
    */

    if (q1.w_ == q2.w_ && q1.x_ == q2.x_ && q1.y_ == q2.y_ && q1.z_ == q2.z_) {
        return q1;
    }

    /*
    * cos(theta) = q1.q2 = (q1.w_ * q2.w_) + (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_)
    **/
    float cos_theta = (q1.w_ * q2.w_) + (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_);

    if (cos_theta < 0.0F) {
        (Quaternion) q2 = Quaternion(-q2.x_, -q2.y_, -q2.z_, -q2.w_);
        cos_theta = -cos_theta;
    }

    float scale_q1 = 1 - t;
    float scale_q2 = t;


    if (1 - cos_theta > 0.1F) {
        float theta = std::acos(cos_theta);
        float sin_theta = std::sin(theta);

        scale_q1 = std::sin((1 - t) * theta) / sin_theta;
        scale_q2 = std::sin(t * theta) / sin_theta;
    }

    Quaternion result;
    result.x_ = (scale_q1 * q1.x_) + (scale_q2 * q2.x_);
    result.y_ = (scale_q1 * q1.y_) + (scale_q2 * q2.y_);
    result.z_ = (scale_q1 * q1.z_) + (scale_q2 * q2.z_);
    result.w_ = (scale_q1 * q1.w_) + (scale_q2 * q2.w_);

    return result;
}


inline float Quaternion::getMagnitud() {
    return std::sqrt(w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_);
}

bool Quaternion::is_unit() {
    return (int) (w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_ - 1.0F) == 0;
}

void Quaternion::To4x4Matrix(float *m4x4) {
    if (!m4x4) {
        return;
    }

    m4x4[0] = 1.0F - 2.0F * (y_ * y_ + z_ * z_);
    m4x4[1] = 2.0F * (x_ * y_ - w_ * z_);
    m4x4[2] = 2.0F * (x_ * z_ + w_ * y_);
    m4x4[3] = 0.0F;

    m4x4[4] = 2.0F * (x_ * y_ + w_ * z_);
    m4x4[5] = 1.0F - 2.0F * (x_ * x_ + z_ * z_);
    m4x4[6] = 2.0F * (y_ * z_ - w_ * x_);
    m4x4[7] = 0.0F;

    m4x4[8] = 2.0F * (x_ * z_ - w_ * y_);
    m4x4[9] = 2.0F * (y_ * z_ + w_ * x_);
    m4x4[10] = 1.0F - 2.0F * (x_ * x_ + y_ * y_);
    m4x4[11] = 0.0F;

    m4x4[12] = 0;
    m4x4[13] = 0;
    m4x4[14] = 0;
    m4x4[15] = 1.0F;

}

void Quaternion::To3x3Matrix(float *m3x3) {
    if (!m3x3) {
        return;
    }

    m3x3[0] = 1.0F - 2.0F * (y_ * y_ + z_ * z_);
    m3x3[1] = 2.0F * (x_ * y_ - w_ * z_);
    m3x3[2] = 2.0F * (x_ * z_ + w_ * y_);

    m3x3[3] = 2.0F * (x_ * y_ + w_ * z_);
    m3x3[4] = 1.0F - 2.0F * (x_ * x_ + z_ * z_);
    m3x3[5] = 2.0F * (y_ * z_ - w_ * x_);

    m3x3[6] = 2.0F * (x_ * z_ - w_ * y_);
    m3x3[7] = 2.0F * (y_ * z_ + w_ * x_);
    m3x3[8] = 1.0F - 2.0F * (x_ * x_ + y_ * y_);
}