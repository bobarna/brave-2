#include "math.h"

std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}