#ifndef BRAVE2_FTL_SIMULATION_H
#define BRAVE2_FTL_SIMULATION_H

#include "Particle.h"
#include "utils/util.h"
class FTL_Simulation {
    size_t n; // number of particles
    float l; // length between each particle
    std::vector<Particle*> particles;
    vec3 color; //color of particles
    vec3 start_pos;
    vec3 external_forces;
public:
    FTL_Simulation(size_t n, float l, vec3 color, vec3 pos);
    /*
     * n: int, number of particles
     * l: float, length between each particle
     * */
    void initialize();
    void add_force(vec3);
    void update(float);
    void draw();
    vec3 get_external_forces();
};


#endif //BRAVE2_FTL_SIMULATION_H
