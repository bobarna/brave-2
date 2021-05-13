#ifndef BRAVE2_SPHSIMULATIONOBJECT_H
#define BRAVE2_SPHSIMULATIONOBJECT_H

#include "../rendering/shaders/Shader.h"
#include "Object.h"
#include "HeadObject.h"
#include "../geometries/SPHSimulation.h"

class SPHSimulationObject : public Object {

public:
    SPHSimulationObject(Shader *_shader, SPHSimulation *_sim);

    void ResetExternalForces();

    void AddForce(vec3 f);

    void Animate(float delta_t) override;
};


#endif //BRAVE2_SPHSIMULATIONOBJECT_H
