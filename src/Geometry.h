#ifndef BRAVE2_GEOMETRY_H
#define BRAVE2_GEOMETRY_H

#include "utils/math.h"
#include <GL/glew.h>

class Geometry {
protected:
    GLuint vao, vbo;
public:

    Geometry() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    };

    virtual void Draw() = 0;

    ~Geometry() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    };
};

#endif //BRAVE2_GEOMETRY_H