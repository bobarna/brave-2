#ifndef BRAVE2_PARAMSURFACE_H
#define BRAVE2_PARAMSURFACE_H


#include "Geometry.h"

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


#endif //BRAVE2_PARAMSURFACE_H
