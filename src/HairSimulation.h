#ifndef BRAVE2_HAIRSIMULATION_H
#define BRAVE2_HAIRSIMULATION_H

#include "utils/math.h"
#include "utils/util.h"
#include "Particle.h"
#include "LinesGeometry.h"
#include "PBD_Strand.h"

class HairSimulation : public LinesGeometry {

public:
    vec3 head;
    size_t nr_sims;
    size_t nr_segments;
    float l_seg;
    std::vector<PBD_Strand> sims;
    vec3 external_forces;

    void add_force_to_all_sims(vec3 force);

    HairSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg);

    void propagateHead();

    void update(float dt);

    void draw();

    vec3 get_external_forces() const;

};


#endif //BRAVE2_HAIRSIMULATION_H
