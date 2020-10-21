#ifndef BRAVE2_FTL_SIMULATION_H
#define BRAVE2_FTL_SIMULATION_H

#include "Particle.h"
class FTL_Simulation {
    size_t n; // number of particles
    float l; // length between each particle
    std::vector<Particle*> particles;
    vec3 color; //color of particles
public:
    FTL_Simulation(size_t n, float l);
    /*
     * n: int, number of particles
     * l: float, length between each particle
     * */
    void setup_simulation();
    void add_force(vec3);
    void update(float);
    void draw();
};


#endif //BRAVE2_FTL_SIMULATION_H
