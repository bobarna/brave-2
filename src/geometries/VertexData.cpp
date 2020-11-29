#include "VertexData.h"

std::ostream &operator<<(std::ostream &out, const VertexData &vD) {
    return out << vD.texcoord << " -> P: " << vD.position << " N: " << vD.normal;
}

VertexData::VertexData(vec3 p, vec3 n, vec2 uv) :
        position(p), normal(n), texcoord(uv) {

}

VertexData::VertexData() {
}
