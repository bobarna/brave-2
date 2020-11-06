//
// Created by bobarna on 2020. 11. 06..
//

#ifndef BRAVE2_LINEOBJECT_H
#define BRAVE2_LINEOBJECT_H


#include "utils/math.h"
#include "Shader.h"
#include "LinesGeometry.h"

class LineObject {
    vec3 scale, translation, rotationAxis;
    float rotationAngle;

    Shader *shader;

public:
    LineObject(Shader *_shader, LinesGeometry* _geometry);

    virtual void SetModelingTransform(mat4 &M, mat4 &Minv);

    void Draw(RenderState state);

    virtual void Animate(float tstart, float tend);

    void Scale(vec3 s);
    void Translate(vec3 t);
    void Rotate(vec3 axis, float angle);
};


#endif //BRAVE2_LINEOBJECT_H
