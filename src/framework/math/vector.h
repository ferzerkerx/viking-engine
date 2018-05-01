#ifndef _VEC_H
#define _VEC_H

#include <cmath>

struct vector2f {
    vector2f() = default;

    vector2f(float S, float T) : s(S), t(T) {

    }

    float s{};
    float t{};
};

struct polar3f {
    polar3f() = default;

    polar3f(float ph, float th, float rad) : phi(ph), theta(th), r(rad) {

    }

    polar3f operator+(polar3f p) {
        return {p.phi + phi, p.theta + theta, p.r + r};
    }

    polar3f operator-(polar3f p) {
        return {phi - p.phi, theta - p.theta, r - p.r};
    }

    polar3f operator*(float num) {
        return {phi * num, theta * num, r * num};
    }

    polar3f operator/(float num) {
        return {phi / num, theta / num, r / num};
    }

    float phi{};
    float theta{};
    float r{};
};


struct vector3f {
    vector3f() = default;

    explicit vector3f(polar3f p) {
        x = p.r * std::sin(p.phi) * std::cos(p.theta);
        y = p.r * std::sin(p.phi) * std::sin(p.theta);
        z = p.r * std::cos(p.phi);
    }

    vector3f(float X, float Y, float Z) : x(X), y(Y), z(Z) {

    }

    vector3f operator+(vector3f vVector) {
        return {vVector.x + x, vVector.y + y, vVector.z + z};
    }

    vector3f operator-(vector3f vVector) {
        return {x - vVector.x, y - vVector.y, z - vVector.z};
    }

    vector3f operator*(float num) {
        return {x * num, y * num, z * num};
    }

    vector3f operator*(vector3f vVector) {
        return {x * vVector.x, y * vVector.y, z * vVector.z};
    }

    vector3f operator/(float num) {
        return {x / num, y / num, z / num};
    }

    float x{};
    float y{};
    float z{};
};

static vector3f Cross(vector3f v1, vector3f v2) {
    vector3f normal;

    normal.x = ((v1.y * v2.z) - (v1.z * v2.y));
    normal.y = ((v1.z * v2.x) - (v1.x * v2.z));
    normal.z = ((v1.x * v2.y) - (v1.y * v2.x));

    return normal;
}

static float Point(vector3f v1, vector3f v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

static float Magnitude(vector3f normalized) {
    // magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)
    return std::sqrt((normalized.x * normalized.x) + (normalized.y * normalized.y) + (normalized.z * normalized.z));
}

static vector3f Normalize(vector3f v1) {
    float magnitude = Magnitude(v1);
    v1 = v1 / magnitude;
    return v1;
}

struct color3f {
    color3f() = default;

    color3f(float R, float G, float B) : r(R), g(G), b(B) {

    }

    color3f operator+(color3f c) {
        return {c.r + r, c.g + g, c.b + b};
    }

    color3f operator-(color3f c) {
        return {r - c.r, g - c.g, b - c.b};
    }

    color3f operator*(float num) {
        return {r * num, g * num, b * num};
    }

    color3f operator*(color3f c) {
        return {r * c.r, g * c.g, b * c.b};
    }

    color3f operator/(float num) {
        return {r / num, g / num, b / num};
    }

    void exposure(float exps) {
        exp = exps;
    }

    float r{};
    float g{};
    float b{};
    float exp{};
};


#endif

