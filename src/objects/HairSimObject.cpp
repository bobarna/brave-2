#include "HairSimObject.h"


void HairSimObject::Animate(float delta_t) {
    reinterpret_cast<PBDSimulation *>(geometry)->update(delta_t);
}

void HairSimObject::ResetExternalForces() {
    reinterpret_cast<PBDSimulation *>(geometry)->resetExternalForces();

}

void HairSimObject::AddForce(vec3 f) {
    reinterpret_cast<PBDSimulation *>(geometry)->addForce(f);
}
