#ifndef BRAVE2_GEOMETRY_H
#define BRAVE2_GEOMETRY_H


#include <GL/glew.h>
#include "utils/math.h"


/**
 * Geometry
 */
class Geometry {
protected:
    unsigned int vao, vbo;
    struct VertexData {
        vec3 position, normal;
        vec2 texcoord;
    };
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


//TODO put this in a better place
const int tessellationLevel = 20;

/**
 * ParamSurface
 */

class ParamSurface : public Geometry {
    unsigned int nVtxPerStrip, nStrips;

public:
    ParamSurface();

    virtual void eval(Dnum2 &U, Dnum2 &V, Dnum2 &X, Dnum2 &Y, Dnum2 &Z) = 0;

    virtual VertexData GenVertexData(float u, float v);

    void create(int N = tessellationLevel, int M = tessellationLevel);

    void Draw();
};

/**
 * Sphere
 */

class Sphere : public ParamSurface {
public:
    Sphere();
    void eval(Dnum2 &U, Dnum2 &V, Dnum2 &X, Dnum2 &Y, Dnum2 &Z);
};

#endif //BRAVE2_GEOMETRY_H
