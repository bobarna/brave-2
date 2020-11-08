#ifndef BRAVE2_CONSTRAINT_H
#define BRAVE2_CONSTRAINT_H


#include "Particle.h"

struct Constraint {
    virtual void solve() = 0;
};

class DistanceConstraint : public Constraint {
    Particle *p1;
    Particle *p2;
    float dist;
    float s; // weight of the constraint

public:
    DistanceConstraint(Particle *p1, Particle *p2, float dist, float s = 1.0f) :
            p1(p1), p2(p2), dist(dist), s(s) {}

    void solve() override {
        vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                    (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                    (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

        vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                    (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                    (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

        p1->tmp_pos += s * d_p1;
        p2->tmp_pos += s * d_p2;
    }
};


//// For now, the same as DistanceConstraint
class BendingConstraint : public Constraint {
    Particle *p1;
    Particle *p2;
    float dist;
    float s; // weight of the constraint

public:
    BendingConstraint(Particle *p1, Particle *p2, float dist, float s = 1.0f) :
            p1(p1), p2(p2), dist(dist), s(s) {}

    void solve() override {
        vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                    (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                    (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
        vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                    (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                    (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

        p1->tmp_pos += s * d_p1;
        p2->tmp_pos += s * d_p2;
    }
};

class CollisionConstraint : public Constraint {
    Particle *p;
    vec3 q1;
    vec3 q2;
    vec3 q3;
    float s = 1.0f;
public:
    CollisionConstraint(Particle *p, vec3 q1, vec3 q2, vec3 q3) :
        p(p), q1(q1), q2(q2), q3(q3){
    }

    void solve() override{
        vec3 n = normalize(cross(q1 - q3, q1 - q2));

        if (dot(p->tmp_pos - q1, n) < 0) return;

        vec3 d_p = -((p->tmp_pos - q2) * n) * n;

        p->tmp_pos += d_p;
    }
};

#endif //BRAVE2_CONSTRAINT_H
