#ifndef BRAVE2_HAIRSIMOBJECT_H
#define BRAVE2_HAIRSIMOBJECT_H

#include "../geometries/PBDSimulation.h"
#include "../rendering/shaders/Shader.h"
#include "Object.h"
#include "HeadObject.h"

class HairSimObject : public Object {
    HeadObject *head;
public:
    HairSimObject(HeadObject *_head, Shader *_shader, PBDSimulation *_sim);

    void ResetExternalForces();

    void AddForce(vec3 f);

    void Animate(float delta_t) override;
};


#endif //BRAVE2_HAIRSIMOBJECT_H
