#include "PBDSimulationObject.h"

PBDSimulationObject::PBDSimulationObject(Shader *_shader, PBDSimulation *_sim) :
        Object(_shader, _sim) {
}

void PBDSimulationObject::Animate(float delta_t) {
    reinterpret_cast<PBDSimulation *>(geometry)->update(delta_t);
}

void PBDSimulationObject::ResetExternalForces() {
    reinterpret_cast<PBDSimulation *>(geometry)->resetExternalForces();
}

void PBDSimulationObject::AddForce(vec3 f) {
    reinterpret_cast<PBDSimulation *>(geometry)->addForce(f);
}