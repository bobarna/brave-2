#include <iostream>
#include "FTL_Simulation.h"

FTL_Simulation::FTL_Simulation(size_t n, float l, vec3 color, vec3 pos) : n(n), l(l), color(color), start_pos(pos) {
    setup_simulation();
}

void FTL_Simulation::setup_simulation() {
    vec3 pos = start_pos;
//    float m = 1.5f;

    for (size_t i = 0; i < n; i++) {
        float m = (float) rand() / RAND_MAX * 3.0f + 1.5f;
        particles.push_back(new Particle(pos, m));
        pos.y -= l;
    }
}

void FTL_Simulation::add_force(vec3 f) {
    // skip the first particle
    for (auto p = ++particles.begin(); p != particles.end(); ++p)
        ((Particle *) *p)->f += f;
}

void FTL_Simulation::update(float dt) {
    // skip the first one
    for (auto it = ++particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        if (it == particles.begin()) {
            p->tmp_pos = p->pos;
            continue;
        }
        p->v = p->v + dt * (p->f / p->m);
        p->tmp_pos += (p->v * dt);
//        p->f = 0;
        p->v *= 0.8;
    }

    // solve constraints
    vec3 dir;
    vec3 curr_pos;
    for (size_t i = 1; i < particles.size(); ++i) {
        Particle *pa = particles[i - 1];
        Particle *pb = particles[i];
        curr_pos = pb->tmp_pos;
        dir = pb->tmp_pos - pa->tmp_pos;
        dir = normalize(dir);
        pb->tmp_pos = pa->tmp_pos + dir * l;
        pb->d = curr_pos - pb->tmp_pos;
    }

    for (size_t i = 1; i < particles.size(); ++i) {
        Particle *pa = particles[i - 1];
        Particle *pb = particles[i];
        pa->v = ((pa->tmp_pos - pa->pos) / dt) + 0.98 * (pb->d / dt);
        pa->pos = pa->tmp_pos;
    }
}

void FTL_Simulation::draw() {
//    glLineWidth(0.04f);
    glPointSize(5.0f);
    glBegin(GL_LINE_STRIP);
    bool flag = true;
    for (auto p = particles.begin(); p != particles.end() - 1; ++p) {
//        if(p == particles.begin()) glColor3f(0.6f, 0.2f, 0.2f);
//        else glColor3f(color.x, color.y, color.z);

        glColor3f(color.x, color.y, color.z);
        flag = !flag;
        vec3 pos = ((Particle *) *p)->pos;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

