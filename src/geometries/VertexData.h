#ifndef BRAVE2_VERTEXDATA_H
#define BRAVE2_VERTEXDATA_H

#include "../utils/math.h"

struct VertexData {
    vec3 position, normal;
    vec2 texcoord;
};

std::ostream &operator<<(std::ostream &out, const VertexData &vD);

#endif //BRAVE2_VERTEXDATA_H
