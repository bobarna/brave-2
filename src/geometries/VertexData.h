#ifndef BRAVE2_VERTEXDATA_H
#define BRAVE2_VERTEXDATA_H

#include "../utils/math.h"

struct VertexData {
    // needed for GetVertexByUV function
    bool valid = true;
    vec3 position, normal;
    vec2 texcoord;
    VertexData();
    VertexData(vec3 p, vec3 n, vec2 uv);
    explicit VertexData(bool valid, vec3 p = {0,0,0}, vec3 n = {0,0,0}, vec2 uv = {0,0}){};
};

std::ostream &operator<<(std::ostream &out, const VertexData &vD);

#endif //BRAVE2_VERTEXDATA_H
