#include "Geometry.h"

Geometry::Geometry() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
};

Geometry::~Geometry() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
};