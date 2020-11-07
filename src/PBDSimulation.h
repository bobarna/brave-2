#ifndef BRAVE2_PBDSIMULATION_H
#define BRAVE2_PBDSIMULATION_H

#include "utils/math.h"
#include "utils/util.h"
#include "Particle.h"
#include "LinesGeometry.h"
#include "PBD_Strand.h"

class PBDSimulation : public LinesGeometry {
public:
    vec3 head;
    size_t nrSims;
    size_t nrSegments;
    float lSeg;
    std::vector<PBD_Strand> sims;
    vec3 externalForces;

    void addForce(vec3 force);

    PBDSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg);

    void propagateHead();

    void update(float dt);

    void draw();

    vec3 getExternalForces() const;

};


#endif //BRAVE2_PBDSIMULATION_H
