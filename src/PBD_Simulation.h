#ifndef BRAVE2_PBD_SIMULATION_H
#define BRAVE2_PBD_SIMULATION_H

#include "Particle.h"

class PBD_Simulation {
    size_t n; // number of particles
    float l; // length between each particle
    std::vector<Particle*> particles;
    vec3 color; //color of particles
    vec3 start_pos;
    vec3 external_forces;

    void solve_distance_constraint(Particle* p1, Particle* p2);
    void solve_position_constraint(Particle *pParticle, Particle *pParticle1);
public:
    PBD_Simulation(size_t n, float l, vec3 color, vec3 pos);
//    ~PBD_Simulation();

    void initialize();
    void add_force(vec3 f);
    void update(float dt);
    void draw();
};


#endif //BRAVE2_PBD_SIMULATION_H
