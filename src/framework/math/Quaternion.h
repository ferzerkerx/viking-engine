#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "vector.h"

/**
* @class Quaternion
* @author Fernando Montes de Oca Cespedes
* @date Sunday, October 28, 2007 11:54:04 PM
*/
class Quaternion {
public:
    Quaternion();

    Quaternion(float w, float x, float y, float z);

    Quaternion(float theta, vector3f *axis);

    Quaternion(float *matrix, int size);

    bool operator==(const Quaternion &q);

    Quaternion operator+(const Quaternion &q);

    Quaternion operator-(const Quaternion &q);

    Quaternion operator*(const Quaternion &q);

    Quaternion operator*(float f);

    vector3f operator*(vector3f vector);

    Quaternion operator/(float f);

    Quaternion &operator+=(const Quaternion &q);

    Quaternion &operator-=(const Quaternion &q);

    Quaternion &operator*=(const Quaternion &q);

    Quaternion &operator*=(float f);

    Quaternion &operator/=(float f);

    vector3f RotateVector(vector3f vector);

    Quaternion Normalize();

    Quaternion Conjugate();

    Quaternion Inverse();

    Quaternion SLERP(const Quaternion &q1, const Quaternion &q2, float t);

    float getMagnitud();

    void To4x4Matrix(float *m4x4);

    void To3x3Matrix(float *m3x3);

    bool is_unit();

    float w_;
    float x_;
    float y_;
    float z_;

};

#endif