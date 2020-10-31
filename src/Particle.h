#ifndef BRAVE2_PARTICLE_H
#define BRAVE2_PARTICLE_H

#include "utils/math.h"

class Particle {
public:
    Particle(vec3 pos, float w): pos(pos), tmp_pos(pos), w(w), v(0.0f) {
        d = vec3(0,0,0);
    };
    vec3 pos;
    vec3 tmp_pos;

    // TODO store only inverse mass of the particle
//    float m; //mass of the particle
    float w; // inverse mass of the particle


    vec3 v; //velocity

    // needed for FTL
    vec3 d; // velocity correction vector


    /** gets the particle's mass
     *
     * @return the particle's mass. If infinity, 0 is returned.
     */
    float get_mass() {
        if (w == 0.0f) return 0;
        return 1/w;
    }
};


#endif //BRAVE2_PARTICLE_H
