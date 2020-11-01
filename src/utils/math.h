#ifndef BRAVE2_MATH_H
#define BRAVE2_MATH_H

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include <GL/glew.h>

/**
 * vec2
 */
struct vec2 {
    float x, y;

    vec2(float x0 = 0, float y0 = 0);

    vec2 operator*(float a) const;

    vec2 &operator*=(const float &a);

    vec2 operator/(float a) const;

    vec2 &operator/=(const float &a);

    vec2 operator+(const vec2 &v) const;

    vec2 &operator+=(const vec2 &rhs);

    vec2 operator-(const vec2 &v) const;

    vec2 &operator-=(const vec2 &rhs);

    vec2 operator*(const vec2 &v) const;

    vec2 &operator*=(const vec2 &rhs);
};

inline float dot(const vec2 &v1, const vec2 &v2) {
    return (v1.x * v2.x + v1.y * v2.y);
}

inline float length(const vec2 &v) { return sqrtf(dot(v, v)); }

inline vec2 normalize(const vec2 &v) { return v * (1 / length(v)); }

inline vec2 operator*(float a, const vec2 &v) { return vec2(v.x * a, v.y * a); }


/**
 * vec3
 */
struct vec3 {
    float x, y, z;

    vec3(float x0 = 0, float y0 = 0, float z0 = 0);

    vec3(vec2 v);

    vec3 operator*(float a) const;

    vec3 operator*=(float const &a);

    vec3 operator/(float a) const;

    vec3 operator/=(float const &a);

    vec3 operator+(const vec3 &v) const;

    vec3 operator+=(const vec3 &v);

    vec3 operator-(const vec3 &v) const;

    vec3 operator-=(const vec3 &v);

    vec3 operator*(const vec3 &v) const;

    vec3 operator*=(const vec3 &v);

    vec3 operator-() const;

};

inline float dot(const vec3 &v1, const vec3 &v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

inline float length(const vec3 &v) { return sqrtf(dot(v, v)); }

inline vec3 normalize(const vec3 &v) { return v * (1 / length(v)); }

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

inline vec3 operator*(float a, const vec3 &v) { return vec3(v.x * a, v.y * a, v.z * a); }

// automatic vec3 printing
std::ostream &operator<<(std::ostream &out, const vec3 &v);

/**
 * vec4
 */
struct vec4 {
    float x, y, z, w;

    vec4(float x0 = 0, float y0 = 0, float z0 = 0, float w0 = 0);

    float &operator[](int j);

    float operator[](int j) const;

    vec4 operator*(float a) const;

    vec4 operator*=(const float &a);

    vec4 operator/(float d) const;

    vec4 operator/=(const float &d);

    vec4 operator+(const vec4 &v) const;

    vec4 operator+=(const vec4 &v);

    vec4 operator-(const vec4 &v) const;

    vec4 operator-=(const vec4 &v);

    vec4 operator*(const vec4 &v) const;

    vec4 operator*=(const vec4 &v);
};

inline float dot(const vec4 &v1, const vec4 &v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline vec4 operator*(float a, const vec4 &v) {
    return vec4(v.x * a, v.y * a, v.z * a, v.w * a);
}


/**
 * mat4
 */
struct mat4 { // row-major matrix 4x4
    vec4 rows[4];
public:
    mat4() {}

    mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33);

    mat4(vec4 it, vec4 jt, vec4 kt, vec4 ot);

    vec4 &operator[](int i);

    vec4 operator[](int i) const;

    operator float *() const;
};

inline vec4 operator*(const vec4 &v, const mat4 &mat) {
    return v[0] * mat[0] + v[1] * mat[1] + v[2] * mat[2] + v[3] * mat[3];
}

inline mat4 operator*(const mat4 &left, const mat4 &right) {
    mat4 result;
    for (int i = 0; i < 4; i++) result.rows[i] = left.rows[i] * right;
    return result;
}

inline mat4 translate_matrix(vec3 t) {
    return mat4(vec4(1, 0, 0, 0),
                vec4(0, 1, 0, 0),
                vec4(0, 0, 1, 0),
                vec4(t.x, t.y, t.z, 1));
}

inline mat4 scale_matrix(vec3 s) {
    return mat4(vec4(s.x, 0, 0, 0),
                vec4(0, s.y, 0, 0),
                vec4(0, 0, s.z, 0),
                vec4(0, 0, 0, 1));
}

inline mat4 rotation_matrix(float angle, vec3 w) {
    float c = cosf(angle), s = sinf(angle);
    w = normalize(w);
    return mat4(vec4(c * (1 - w.x * w.x) + w.x * w.x, w.x * w.y * (1 - c) + w.z * s, w.x * w.z * (1 - c) - w.y * s, 0),
                vec4(w.x * w.y * (1 - c) - w.z * s, c * (1 - w.y * w.y) + w.y * w.y, w.y * w.z * (1 - c) + w.x * s, 0),
                vec4(w.x * w.z * (1 - c) + w.y * s, w.y * w.z * (1 - c) - w.x * s, c * (1 - w.z * w.z) + w.z * w.z, 0),
                vec4(0, 0, 0, 1));
}

inline mat4 x_rotation_matrix(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    return mat4(
            1.f, 0.f, 0.f, 0.f,
            0.f, c, s, 0.f,
            0.f, -s, c, 0.f,
            0.f, 0.f, 0.f, 1.f
    );
}

inline mat4 y_rotation_matrix(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    return mat4(
            c, 0.f, -s, 0.f,
            0.f, 1.f, 0.f, 0.f,
            s, 0.f, c, 0.f,
            0.f, 0.f, 0.f, 1.f
    );
}

inline mat4 z_rotation_matrix(float angle) {
    float s = sinf(angle);
    float c = cosf(angle);
    return mat4(
            c, s, 0.f, 0.f,
            -s, c, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f
    );
}

/**
 *  Dnum class
 * @tparam T
 * @param f0
 * @param d0
 */
//---------------------------
template<class T>
struct Dnum { // Dual numbers for automatic derivation
//---------------------------
    float f; //function value
    T d; //derivatives
    Dnum(float f0 = 0, T d0 = T(0)) { f = f0, d = d0; }

    Dnum operator+(Dnum r) { return Dnum(f + r.f, d + r.d); }

    Dnum operator-(Dnum r) { return Dnum(f - r.f, d - r.d); }

    Dnum operator*(Dnum r) {
        return Dnum(f * r.f, f * r.d + d * r.f);
    }

    Dnum operator/(Dnum r) {
        return Dnum(f / r.f, (r.f * d - r.d * f) / r.f / r.f);
    }
};

template<class T>
Dnum<T> C(float t) { return Dnum<T>(t, 1); }

template<class T>
Dnum<T> Exp(Dnum<T> g) { return Dnum<T>(expf(g.f), expf(g.f) * g.d); }

template<class T>
Dnum<T> Sin(Dnum<T> g) { return Dnum<T>(sinf(g.f), cosf(g.f) * g.d); }

template<class T>
Dnum<T> Cos(Dnum<T> g) { return Dnum<T>(cosf(g.f), -sinf(g.f) * g.d); }

template<class T>
Dnum<T> Tan(Dnum<T> g) { return Sin(g) / Cos(g); }

template<class T>
Dnum<T> Sinh(Dnum<T> g) { return Dnum<T>(sinh(g.f), cosh(g.f) * g.d); }

template<class T>
Dnum<T> Cosh(Dnum<T> g) { return Dnum<T>(cosh(g.f), sinh(g.f) * g.d); }

template<class T>
Dnum<T> Tanh(Dnum<T> g) { return Sinh(g) / Cosh(g); }

template<class T>
Dnum<T> Log(Dnum<T> g) { return Dnum<T>(logf(g.f), g.d / g.f); }

template<class T>
Dnum<T> Pow(Dnum<T> g, float n) {
    return Dnum<T>(powf(g.f, n), n * powf(g.f, n - 1) * g.d);
}

typedef Dnum<vec2> Dnum2;

#endif //BRAVE2_MATH_H
