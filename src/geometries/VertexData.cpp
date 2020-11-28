#include "VertexData.h"

std::ostream &operator<<(std::ostream &out, const VertexData &vD) {
    return out << vD.texcoord << " -> P: " << vD.position << " N: " << vD.normal;
}
