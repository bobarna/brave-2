#ifndef BRAVE2_PBDSIMULATIONOBJECT_H
#define BRAVE2_PBDSIMULATIONOBJECT_H

#include "../geometries/PBDSimulation.h"
#include "../rendering/shaders/Shader.h"
#include "Object.h"
#include "HeadObject.h"

class PBDSimulationObject : public Object {

public:
    PBDSimulationObject(Shader *_shader, PBDSimulation *_sim, Material* _material, Texture* _texture);

    PBDSimulationObject(Shader *_shader, PBDSimulation *_sim);

    void ResetExternalForces();

    void AddForce(vec3 f);

    void Animate(float delta_t) override;
};


#endif //BRAVE2_PBDSIMULATIONOBJECT_H
