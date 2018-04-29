#ifndef _VEC_H
#define _VEC_H

#include <math.h>

struct vector2f {
    vector2f() {}    // Constructor

    vector2f(float S, float T) {    // Constructor inicializador
        s = S;
        t = T;
    }

    float s, t;
};

struct polar3f {
    polar3f() {}    // Constructor
    polar3f(float ph, float th, float rad) {    // Constructor inicializador
        phi = ph;
        theta = th;
        r = rad;
    }

    // Operaciones con vectores
    polar3f operator+(polar3f p) {
        return polar3f(p.phi + phi, p.theta + theta, p.r + r);
    }

    polar3f operator-(polar3f p) {
        return polar3f(phi - p.phi, theta - p.theta, r - p.r);
    }

    polar3f operator*(float num) {
        return polar3f(phi * num, theta * num, r * num);
    }

    polar3f operator/(float num) {
        return polar3f(phi / num, theta / num, r / num);
    }

    float phi, theta, r;
};


struct vector3f {
    vector3f() {}    // Constructor

    vector3f(polar3f p) {    // Constructor inicializador
        x = p.r * sin(p.phi) * cos(p.theta);
        y = p.r * sin(p.phi) * sin(p.theta);
        z = p.r * cos(p.phi);
    }

    vector3f(float X, float Y, float Z) {    // Constructor inicializador
        x = X;
        y = Y;
        z = Z;
    }

    // Operaciones con vectores
    vector3f operator+(vector3f vVector) {
        return vector3f(vVector.x + x, vVector.y + y, vVector.z + z);
    }

    vector3f operator-(vector3f vVector) {
        return vector3f(x - vVector.x, y - vVector.y, z - vVector.z);
    }

    vector3f operator*(float num) {
        return vector3f(x * num, y * num, z * num);
    }

    vector3f operator*(vector3f vVector) {
        return vector3f(x * vVector.x, y * vVector.y, z * vVector.z);
    }

    vector3f operator/(float num) {
        return vector3f(x / num, y / num, z / num);
    }

    float x, y, z;
};

// Regresa el producto cruzado entre 2 vectores
static vector3f Cruzado(vector3f vVector1, vector3f vVector2) {
    vector3f vNormal;

    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    return vNormal;
}

// regresa el producto punto entre 2 vectores
static float Punto(vector3f v1, vector3f v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Regresa la magnitud del vector
static float Magnitud(vector3f vNorm) {
    // magnitud = sqrt(V.x^2 + V.y^2 + V.z^2)
    return (float) sqrt((vNorm.x * vNorm.x) + (vNorm.y * vNorm.y) + (vNorm.z * vNorm.z));
}

// Regresa vector normalizado (vector de longitud 1)
static vector3f Normalizar(vector3f vVector) {
    float magnitud = Magnitud(vVector);
    vVector = vVector / magnitud;
    return vVector;
}

struct color3f {
    color3f() {}    // Constructor
    color3f(float R, float G, float B) {    // Constructor inicializador
        r = R;
        g = G;
        b = B;
    }

    // Operaciones con vectores
    color3f operator+(color3f c) {
        return color3f(c.r + r, c.g + g, c.b + b);
    }

    color3f operator-(color3f c) {
        return color3f(r - c.r, g - c.g, b - c.b);
    }

    color3f operator*(float num) {
        return color3f(r * num, g * num, b * num);
    }

    color3f operator*(color3f c) {
        return color3f(r * c.r, g * c.g, b * c.b);
    }

    color3f operator/(float num) {
        return color3f(r / num, g / num, b / num);
    }

    void exposure(float exps) {
        exp = exps;
    }

    float r, g, b;
    float exp;
};


#endif

