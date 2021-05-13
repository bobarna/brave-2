#ifndef BRAVE2_PARTICLE_H
#define BRAVE2_PARTICLE_H

#include "../utils/math.h"

class Particle {
public:
    Particle(vec3 pos, float w, vec3 c) : pos(pos), tmp_pos(pos), w(w), v(0.0f), color(c) {

    };

    Particle(vec3 pos): pos(pos) {

    };

    vec3 pos;
    vec3 tmp_pos;

    float w; // inverse mass of the particle

    vec3 v; //velocity

    vec3 color;

    // For SPH simulation
    float p;            // pressure (varies)
    float rho;          // density (varies)
    vec3 f;           // current sum of external forces acting on the particle


    /** gets the particle's mass
     *
     * @return the particle's mass. If infinity, -1 is returned.
     */
    float get_mass() {
        if (w == 0.0f) return -1;
        return 1 / w;
    }
};


#endif //BRAVE2_PARTICLE_H
