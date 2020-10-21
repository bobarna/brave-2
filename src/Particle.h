#ifndef BRAVE2_PARTICLE_H
#define BRAVE2_PARTICLE_H

#include "utils/math.h"

class Particle {
    Particle(vec3 pos, float m): pos(pos), tmp_pos(pos), m(m) {};
    vec3 pos;
    vec3 tmp_pos;
    vec3 f; //forces
    vec3 v; //velocity
    vec3 d; // velocity correction vector
    float m; //mass of the particle
};


#endif //BRAVE2_PARTICLE_H
