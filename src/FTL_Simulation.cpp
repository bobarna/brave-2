#include <iostream>
#include "FTL_Simulation.h"

FTL_Simulation::FTL_Simulation(size_t n, float l, vec3 color, vec3 pos) : n(n), l(l), color(color), start_pos(pos) {
    initialize();
}

void FTL_Simulation::initialize() {
    vec3 pos = start_pos;
    for (size_t i = 0; i < n; i++) {
        //random mass
        float m = util::randomOffsetf(1.5f, 3.0f);
//        float m = 1.5f;

        // first particle's position is infinite
        if(i == 0) particles.push_back(new Particle(pos, 0.0f));
        else particles.push_back(new Particle(pos, 1/m));

        // propagate particles downward
        pos.y -= l;
    }
}

void FTL_Simulation::add_force(vec3 f) {
    external_forces += f;
}

void FTL_Simulation::update(float dt) {
    // skip the first one
    for (auto it = ++particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        // TODO rework forces acting on current particle
        p->v = p->v + dt * external_forces * p->w;
        p->tmp_pos += p->v * dt;
//        p->v *= 0.99;
    }

    // solve constraints
    vec3 dir;
    vec3 proposed_pos;
    for (size_t i = 1; i < particles.size(); ++i) {
        Particle *pa = particles[i - 1];
        Particle *pb = particles[i];
        proposed_pos = pb->tmp_pos;
        dir = pb->tmp_pos - pa->tmp_pos;
        dir = normalize(dir);
        pb->tmp_pos = pa->tmp_pos + dir * l;
        pb->d =  pb->tmp_pos - proposed_pos;
    }

    for (size_t i = 1; i < particles.size(); ++i) {
        Particle *pa = particles[i - 1];
        Particle *pb = particles[i];
        // (9) in FTL paper
        if(i == 1) continue;

        pa->v = ((pa->tmp_pos - pa->pos) / dt) - 0.94 * (pb->d / dt);
        pa->pos = pa->tmp_pos;
    }

    Particle* last = particles.back();
    last->pos = last->tmp_pos;
}

void FTL_Simulation::draw() {
    glLineWidth(1.5f);
    glPointSize(5.0f);
    glBegin(GL_LINE_STRIP);
//    glBegin(GL_POINTS);
    bool flag = true;
    for (auto p = particles.begin(); p != particles.end() ; ++p) {
        glColor3f(color.x, color.y, color.z);
        flag = !flag;
        vec3 pos = ((Particle *) *p)->pos;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

vec3 FTL_Simulation::get_external_forces() {
    for(auto& p : particles) {
        p->d = 0;
    }

    return external_forces;
}

