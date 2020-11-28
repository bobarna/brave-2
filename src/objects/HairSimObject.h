#ifndef BRAVE2_HAIRSIMOBJECT_H
#define BRAVE2_HAIRSIMOBJECT_H

#include "PBDSimulation.h"

class HairSimObject : public Object {
public:
    //TODO why can't I refactor this to .cpp file???
    HairSimObject(Shader *_shader, PBDSimulation *_sim) : Object(_shader, _sim) {};

    void ResetExternalForces();

    void AddForce(vec3 f);

    void Animate(float delta_t) override;
};



#endif //BRAVE2_HAIRSIMOBJECT_H
