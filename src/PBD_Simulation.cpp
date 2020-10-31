#include "PBD_Simulation.h"
#include "utils/util.h"

PBD_Simulation::PBD_Simulation(size_t n, float l, vec3 color, vec3 pos) :
        n(n), l(l), color(color), start_pos(pos) {
    initialize();
}

void PBD_Simulation::initialize() {
    vec3 pos = start_pos;
    for (size_t i = 0; i < n; i++) {
        // random mass
        // A single strand of hair can weigh between .2 – .5 milligrams,
        // which is 2.0e-7 kg == 10^(-7) kg
        float m = util::randomOffsetf(.35f, .15f);

        // first particle's position is infinite
        if(i == 0) particles.push_back(new Particle(pos, 0));
        else particles.push_back(new Particle(pos, 1/m));

        // propagate particles downward
        pos.y -= l;
    }
}

void PBD_Simulation::add_force(vec3 f) {
    external_forces += f;
}

void PBD_Simulation::update(float dt) {
    // TODO Ez a publikáció hibás (4.2-es bekezdés)
    // http://cg.iit.bme.hu/~umitomi/publications/GRAFGEO2016_PhysicsSimulators.pdf

    // setting velocity
    for(auto it = particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        p->v = p->v + dt * (p->w) * external_forces;

        // damp velocities
        // TODO better damping technique: (3.5) https://matthias-research.github.io/pages/publications/posBasedDyn.pdf
        p->v *= .99f;
    }

    //calculating temporal positions
    //skipping first one
    for(auto it = particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        p->tmp_pos = p->pos + dt * p->v;
    }

    // solve distance constraints
    // TODO find out ideal number of iterations
    size_t num_iter = 15;
    for (size_t iter = 0; iter < num_iter; iter++) {
        //keep first particle in place

        particles[0]->tmp_pos = particles[0]->pos;
        //distance between subsequent particles should be l
        for (size_t i = 1; i < particles.size(); i++)
            solve_distance_constraint(particles[i - 1], particles[i]);
    }

    // updating velocities and positions
    // skipping first one
    // it stays in place
    for(auto it = ++particles.begin(); it != particles.end(); ++it){
        Particle* p = *it;
        p->v = (p->tmp_pos - p->pos) / dt;
        p->pos = p->tmp_pos;
    }
}

//distances should be l
void PBD_Simulation::solve_distance_constraint(Particle *p1, Particle *p2) {

    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - l) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - l) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);;

    p1->tmp_pos += d_p1;
    p2->tmp_pos += d_p2;
}

void PBD_Simulation::draw() {
    glLineWidth(1.6f);
    glPointSize(5.0f);
    glBegin(GL_LINE_STRIP);
//    glBegin(GL_POINTS);
    bool flag = true;
    for (auto p = particles.begin(); p != particles.end() - 1; ++p) {
        glColor3f(color.x, color.y, color.z);
        flag = !flag;
        vec3 pos = ((Particle *) *p)->pos;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

vec3 PBD_Simulation::get_external_forces() {
    return external_forces;
}