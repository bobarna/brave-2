#ifndef BRAVE2_HAIRSIMULATION_H
#define BRAVE2_HAIRSIMULATION_H

#include "utils/math.h"
#include "utils/util.h"
#include "Particle.h"

class HairSimulation {
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
    };
public:
    vec3 head;
    size_t nr_sims;
    size_t nr_segments;
    float l_seg;
    std::vector<PBD_Strand> sims;
    vec3 external_forces;

    void add_force_to_all_sims(vec3 force);

    HairSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg);

    void propagateHead();

    void update(float dt);

    void draw();

    vec3 get_external_forces() const;

};


#endif //BRAVE2_HAIRSIMULATION_H
