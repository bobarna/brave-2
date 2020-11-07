#ifndef BRAVE2_HAIRSIMULATIONOBJECT_H
#define BRAVE2_HAIRSIMULATIONOBJECT_H


#include "utils/math.h"
#include "Shader.h"
#include "LinesGeometry.h"
#include "RenderState.h"
#include "HairSimulation.h"

class HairSimulationObject {
    vec3 scale, translation, rotationAxis;
    float rotationAngle;

    Shader *shader;
    HairSimulation* sim;

public:
    HairSimulationObject(Shader *_shader, HairSimulation* _sim);

    virtual void SetModelingTransform(mat4 &M, mat4 &Minv);

    void Draw(RenderState state);

    virtual void Animate(float delta_t);

    void Scale(vec3 s);
    void Translate(vec3 t);
    void Rotate(vec3 axis, float angle);
};


#endif //BRAVE2_HAIRSIMULATIONOBJECT_H
