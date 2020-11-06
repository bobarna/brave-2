#ifndef BRAVE2_LINESGEOMETRY_H
#define BRAVE2_LINESGEOMETRY_H

#include "utils/math.h"
#include <GL/glew.h>

class LinesGeometry {
protected:
    unsigned int vao, vbo;
public:

    LinesGeometry() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    };

    virtual void draw() = 0;

    ~LinesGeometry() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    };
};

#endif //BRAVE2_LINESGEOMETRY_H