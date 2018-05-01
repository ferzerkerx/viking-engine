/**
* @file Quaternion.h
* @brief Encabezado de la clase Quaternion
* @date Friday, October 26, 2007 8:06:39 PM
*/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "vector.h"

/**
* @class Quaternion
* @author Fernando Montes de Oca Cespedes
* @date Sunday, October 28, 2007 11:54:04 PM
* @brief Clase que maneja los Quaterniones, hace SLERP y 
* rotacion de vectores
* @todo quitar m_buff y toString para el release final
*/
class Quaternion {
public:
    Quaternion();

    Quaternion(float w, float x, float y, float z);

    Quaternion(float theta, vector3f *eje);

    Quaternion(float *matriz, int num_columnas);

    bool operator==(const Quaternion &q);

    Quaternion operator+(const Quaternion &q);

    Quaternion operator-(const Quaternion &q);

    Quaternion operator*(const Quaternion &q);

    Quaternion operator*(float f);

    vector3f operator*(vector3f p);

    Quaternion operator/(float f);

    Quaternion &operator+=(const Quaternion &q);

    Quaternion &operator-=(const Quaternion &q);

    Quaternion &operator*=(const Quaternion &q);

    Quaternion &operator*=(float f);

    Quaternion &operator/=(float f);

    vector3f rotaVector(vector3f p);

    Quaternion normaliza();

    Quaternion getConjugado();

    Quaternion getInversa();

    Quaternion SLERP(const Quaternion &q1, const Quaternion &q2, float t);

    float getMagnitud();

    void to4x4Matriz(float *m4x4);

    void to3x3Matriz(float *m3x3);

    bool isUnitario();

    char *toString();

    float w; //componente real del Quaternion
    float x; //componente imaginario i del Quaternion
    float y; //componente imaginario j del Quaternion
    float z; //componente imaginario k del Quaternion

};

/**
* @var Quaternion::m_buff
* @brief usada para debug
*/

/**
* @var Quaternion::w
* @brief componente real del Quaternion
*/

/**
* @var Quaternion::x
* @brief componente imaginario i del Quaternion
*/

/**
* @var Quaternion::y
* @brief componente imaginario j del Quaternion
*/

/**
* @var Quaternion::z
* @brief componente imaginario k del Quaternion
*/




#endif