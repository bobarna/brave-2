#include "SPHSimulationObject.h"

SPHSimulationObject::SPHSimulationObject(Shader *_shader, SPHSimulation *_sim) :
        Object(_shader, _sim) {
}

void SPHSimulationObject::Animate(float delta_t) {
    reinterpret_cast<SPHSimulation *>(geometry)->update(delta_t);
}

void SPHSimulationObject::ResetExternalForces() {
    reinterpret_cast<SPHSimulation *>(geometry)->resetExternalForces();
}

void SPHSimulationObject::AddForce(vec3 f) {
    reinterpret_cast<SPHSimulation *>(geometry)->addForce(f);
}