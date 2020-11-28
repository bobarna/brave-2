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

    virtual VertexData GetVertexDataByUV(float u, float v) {throw std::bad_exception();};

    ~Geometry();
};

#endif //BRAVE2_GEOMETRY_H