#ifndef BRAVE2_PARTICLE_H
#define BRAVE2_PARTICLE_H

#include "utils/math.h"

class Particle {
public:
    Particle(vec3 pos, float m): pos(pos), tmp_pos(pos), m(m), w(1/m), v(0.0f) {};
    vec3 pos;
    vec3 tmp_pos;

    float m; //mass of the particle
    float w; // inverse mass of the particle

    vec3 v; //velocity

    vec3 f; //forces
    vec3 d; // velocity correction vector
};


#endif //BRAVE2_PARTICLE_H
