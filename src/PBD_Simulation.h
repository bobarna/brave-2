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

    std::vector<vec3> coll;

    void solve_distance_constraint(Particle* p1, Particle* p2);
    void solve_position_constraint(Particle *pParticle, Particle *pParticle1);
    /// p particle collides with triangle(q1, q2, q3)
    void solve_collision_constraint(Particle *p, vec3 &q1, vec3 &q2, vec3 &q3);
    void solve_particle_collision_constraint(Particle *p1, Particle* p2);

public:
    PBD_Simulation(size_t n, float l, vec3 color, vec3 pos);
    //TODO write destructor

    void initialize();
    void add_force(vec3 f);
    void update(float dt);
    void draw();
    vec3 get_external_forces();
};


#endif //BRAVE2_PBD_SIMULATION_H
