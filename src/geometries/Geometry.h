#ifndef BRAVE2_GEOMETRY_H
#define BRAVE2_GEOMETRY_H

#include "../utils/math.h"
#include <GL/glew.h>
#include "VertexData.h"

class Geometry {
protected:
    GLuint vao, vbo;

public:
    std::vector<VertexData> vtxData;    // vertices on the CPU

    Geometry();

    virtual void Draw() = 0;

    ~Geometry();
};

#endif //BRAVE2_GEOMETRY_H