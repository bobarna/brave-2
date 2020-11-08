#ifndef BRAVE2_OBJECT_H
#define BRAVE2_OBJECT_H


#include "utils/math.h"
#include "Shader.h"
#include "Geometry.h"
#include "RenderState.h"
#include "PBDSimulation.h"

class Object {
protected:
    vec3 scale, translation, rotationAxis;
    float rotationAngle;

    Geometry* geometry;
    Shader* shader;

public:
    Object(Shader *_shader, Geometry* _sim);

    virtual void SetModelingTransform(mat4 &M, mat4 &Minv);

    void Draw(RenderState state);

    virtual void Animate(float delta_t);

    void Scale(vec3 s);
    void Translate(vec3 t);
    void Rotate(vec3 axis, float angle);
};

class HairSimObject : public Object {
public:
    HairSimObject(Shader *_shader, PBDSimulation *_sim): Object(_shader, _sim){};
    void ResetExternalForces();

    void AddForce(vec3 f);
};


#endif //BRAVE2_OBJECT_H
