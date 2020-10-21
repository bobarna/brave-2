#include "FTL_Simulation.h"

FTL_Simulation::FTL_Simulation(size_t n, float l): n(n), l(l), color(0.1f, 0.1f, 0.7f) {
    setup_simulation();
}

void FTL_Simulation::setup_simulation() {
    vec3 pos(0.0f, 0.8, 1.0f);
    float m = 1.0f;
    for(size_t i = 0; i < n; i++) {
        particles.push_back(new Particle(pos, m));
        pos.x += l;
    }
}

void FTL_Simulation::add_force(vec3 f) {
    // skip the first particle
    for(auto p = ++particles.begin(); p != particles.end(); ++p)
        ((Particle*)*p)->f += f;
}

void FTL_Simulation::update(float dt) {
    // skip the first one
    for(auto it = ++particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        vec3 tmp_pos = p->pos + dt * p->v + dt*dt*p->f;
        // solve constraints
        auto prev = *(it-1);
        p->pos = l * normalize(p->pos - prev->pos);
        p->d = p->pos - p->tmp_pos ;
        p->v = p->v + dt * (p->f / p->m);
        p->tmp_pos += p->v * dt;
        p->f = 0;
        p->v *= 0.99;
    }

    // solve constraints
    vec3 dir;
    vec3 curr_pos;
    for(size_t i = 1; i< particles.size(); ++i) {
        Particle* prev = particles[i-1];
        Particle* curr = particles[i];

        curr_pos = curr->tmp_pos;
        dir = curr->tmp_pos - prev->tmp_pos;
        normalize(dir);
        curr->tmp_pos = prev->tmp_pos + dir*l;
        curr->d = curr_pos - curr->tmp_pos;
    }

    for(size_t i = 2; i < particles.size(); ++i) {
        Particle* prev = particles[i-1];
        Particle* curr = particles[i];
        //think through -1*
        prev->v = ((prev->tmp_pos - prev->pos)/dt) + 0.9 * (curr->d / dt);
        prev->pos = curr->tmp_pos;
    }

    Particle* last = particles.back();
    last->pos = last->tmp_pos;
}

void FTL_Simulation::draw() {
//    glLineWidth(0.04f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for(auto p = particles.begin(); p != particles.end(); ++p) {
        if(p == particles.begin()) glColor3f(0.6f, 0.2f, 0.2f);
        else glColor3f(color.x, color.y, color.z);
        vec3 pos = ((Particle*)*p)->pos;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

