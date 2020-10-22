#ifndef BRAVE2_FTL_SIMULATION_H
#define BRAVE2_FTL_SIMULATION_H

#include "Particle.h"
class FTL_Simulation {
    size_t n; // number of particles
    float l; // length between each particle
    std::vector<Particle*> particles;
    vec3 color; //color of particles
    vec3 start_pos;
public:
    FTL_Simulation(size_t n, float l, vec3 color, vec3 pos);
    /*
     * n: int, number of particles
     * l: float, length between each particle
     * */
    void setup_simulation();
    void add_force(vec3);
    void update(float);
    void draw();
    vec3 getForce() {return particles[0]->f;}
};


#endif //BRAVE2_FTL_SIMULATION_H
