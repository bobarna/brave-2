//
// Created by bobarna on 2020. 11. 06..
//

#ifndef BRAVE2_PBD_STRAND_H
#define BRAVE2_PBD_STRAND_H


#include "utils/math.h"
#include "Particle.h"

class PBD_Strand {
    size_t n; // number of particles
    float l; // length between each particle
    std::vector<Particle*> particles;
    vec3 color; //color of particles
    vec3 start_pos;
    vec3 external_forces;

    std::vector<vec3> coll;

    void solve_distance_constraint(Particle* p1, Particle* p2, float dist);
    void solve_position_constraint(Particle *pParticle, Particle *pParticle1);
    void solve_bending_constraint(Particle *p1, Particle *p2, float dist);
    /// p particle collides with triangle(q1, q2, q3)
    void solve_collision_constraint(Particle *p, vec3 &q1, vec3 &q2, vec3 &q3);
    void solve_particle_collision_constraint(Particle *p1, Particle* p2);

public:
    PBD_Strand(size_t n, float l, vec3 color, vec3 pos);
    //TODO write destructor

    void initialize();
    void add_force(vec3 f);
    void update(float dt);
    void draw();
    vec3 get_external_forces();

    void getParticlesAsLines(std::vector<float> &vector) {
        for (size_t i = 0; i < particles.size() - 1; ++i) {
            vector.push_back(particles[i]->pos.x);
            vector.push_back(particles[i]->pos.y);
            vector.push_back(particles[i]->pos.z);
            vector.push_back(color.x);
            vector.push_back(color.y);
            vector.push_back(color.z);

            vector.push_back(particles[i+1]->pos.x);
            vector.push_back(particles[i+1]->pos.y);
            vector.push_back(particles[i+1]->pos.z);
            vector.push_back(color.x);
            vector.push_back(color.y);
            vector.push_back(color.z);
        }
    }
};


#endif //BRAVE2_PBD_STRAND_H
