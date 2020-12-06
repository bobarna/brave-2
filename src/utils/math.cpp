#include "math.h"

std::ostream &operator<<(std::ostream &out, const vec2 &v) {
    return out << "(" << v.x << ", " << v.y << ")";
}

std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

std::ostream &operator<<(std::ostream &out, const vec4 &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}

std::ostream &operator<<(std::ostream &out, const mat4 &m) {
    return out << m.rows[0] << std::endl <<
               m.rows[1] << std::endl <<
               m.rows[2] << std::endl <<
               m.rows[3] << std::endl;
}