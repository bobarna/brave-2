#include "math.h"

std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

vec2::vec2(float x0, float y0) {
    x = x0;
    y = y0;
}

vec2 vec2::operator*(float a) const {
    return vec2(x * a, y * a);
}

vec2 &vec2::operator*=(const float &a) {
    (*this) = (*this) * a;
    return *this;
}

vec2 vec2::operator/(float a) const {
    return vec2(x / a, y / a);
}

vec2 &vec2::operator/=(const float &a) {
    (*this) = (*this) / a;
    return *this;
}

vec2 vec2::operator+(const vec2 &v) const {
    return vec2(x + v.x, y + v.y);
}

vec2 &vec2::operator+=(const vec2 &rhs) {
    (*this) = (*this) + rhs;
    return *this;
}

vec2 vec2::operator-(const vec2 &v) const {
    return vec2(x - v.x, y - v.y);
}

vec2 &vec2::operator-=(const vec2 &rhs) {
    (*this) = (*this) - rhs;
    return *this;
}

vec2 vec2::operator*(const vec2 &v) const {
    return vec2(x * v.x, y * v.y);
}

vec2 &vec2::operator*=(const vec2 &rhs) {
    (*this) = (*this) * rhs;
    return *this;
}

vec3::vec3(float x0, float y0, float z0) {
    x = x0;
    y = y0;
    z = z0;
}

vec3::vec3(vec2 v) {
    x = v.x;
    y = v.y;
    z = 0;
}

vec3 vec3::operator*(float a) const {
    return vec3(x * a, y * a, z * a);
}

vec3 vec3::operator*=(const float &a) {
    (*this) = (*this) * a;
    return *this;
}

vec3 vec3::operator/(float a) const {
    return vec3(x / a, y / a, z / a);
}

vec3 vec3::operator/=(const float &a) {
    (*this) = (*this) / a;
    return *this;
}

vec3 vec3::operator+(const vec3 &v) const {
    return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator+=(const vec3 &v) {
    (*this) = (*this) + v;
    return *this;
}

vec3 vec3::operator-(const vec3 &v) const {
    return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator-=(const vec3 &v) {
    (*this) = (*this) - v;
    return *this;
}

vec3 vec3::operator*(const vec3 &v) const {
    return vec3(x * v.x, y * v.y, z * v.z);
}

vec3 vec3::operator*=(const vec3 &v) {
    (*this) = (*this) * v;
    return *this;
}

vec3 vec3::operator-() const {
    return vec3(-x, -y, -z);
}

vec4::vec4(float x0, float y0, float z0, float w0) {
    x = x0;
    y = y0;
    z = z0;
    w = w0;
}

float &vec4::operator[](int j) {
    return *(&x + j);
}

float vec4::operator[](int j) const {
    return *(&x + j);
}

vec4 vec4::operator*(float a) const {
    return vec4(x * a, y * a, z * a, w * a);
}

vec4 vec4::operator*=(const float &a) {
    (*this) = (*this) * a;
    return *this;
}

vec4 vec4::operator/(float d) const {
    return vec4(x / d, y / d, z / d, w / d);
}

vec4 vec4::operator/=(const float &d) {
    (*this) = (*this) * d;
    return *this;
}

vec4 vec4::operator+(const vec4 &v) const {
    return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator+=(const vec4 &v) {
    (*this) = (*this) + v;
    return *this;
}

vec4 vec4::operator-(const vec4 &v) const {
    return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4 vec4::operator-=(const vec4 &v) {
    (*this) = (*this) - v;
    return *this;
}

vec4 vec4::operator*(const vec4 &v) const {
    return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

vec4 vec4::operator*=(const vec4 &v) {
    (*this) = (*this) * v;
    return *this;
}


mat4::mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21,
           float m22, float m23, float m30, float m31, float m32, float m33) {
    rows[0][0] = m00;
    rows[0][1] = m01;
    rows[0][2] = m02;
    rows[0][3] = m03;
    rows[1][0] = m10;
    rows[1][1] = m11;
    rows[1][2] = m12;
    rows[1][3] = m13;
    rows[2][0] = m20;
    rows[2][1] = m21;
    rows[2][2] = m22;
    rows[2][3] = m23;
    rows[3][0] = m30;
    rows[3][1] = m31;
    rows[3][2] = m32;
    rows[3][3] = m33;
}

mat4::mat4(vec4 it, vec4 jt, vec4 kt, vec4 ot) {
    rows[0] = it;
    rows[1] = jt;
    rows[2] = kt;
    rows[3] = ot;
}

vec4 &mat4::operator[](int i) {
    return rows[i];
}

vec4 mat4::operator[](int i) const {
    return rows[i];
}

mat4::operator float *() const {
    return (float *) this;
}

/**
 * Dnum
 */



