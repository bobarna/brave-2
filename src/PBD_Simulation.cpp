#include "PBD_Simulation.h"
#include "utils/util.h"

PBD_Simulation::PBD_Simulation(size_t n, float l, vec3 color, vec3 pos) :
        n(n), l(l), color(color), start_pos(pos) {
    initialize();
}

//PBD_Simulation::~PBD_Simulation() {
//    //TODO check if this is a working destructor
//    for (auto it = particles.begin(); it != particles.end(); ++it)
//        delete (*it);
//}

void PBD_Simulation::initialize() {
    vec3 pos = start_pos;
    for (size_t i = 0; i < n; i++) {
        //random mass
        float m = util::randomOffsetf(1.5f, 3.0f);
        if(i == 0) m = 0.0000000001;
        particles.push_back(new Particle(pos, m));
        pos.y -= l;
    }

    // first particle has "infinite" mass

}

void PBD_Simulation::add_force(vec3 f) {
    external_forces += f;
}

void PBD_Simulation::update(float dt) {
    // Based on http://cg.iit.bme.hu/~umitomi/publications/GRAFGEO2016_PhysicsSimulators.pdf

    // setting velocity
    // skipping first one
    for(auto it = particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        p->v = p->v + dt * (p->w) * external_forces;
    }

    //calculating temporal positions
    //skipping first one
    for(auto it = particles.begin(); it != particles.end(); ++it) {
        Particle *p = *it;
        p->tmp_pos = p->pos + dt * p->v;
    }

    // solve distance constraints
    // TODO find out ideal number of iterations
    size_t num_iter = 50;
    for (size_t iter = 0; iter < num_iter; iter++) {
        //keep first particle in place
        // TODO find a better way to do this
        // TODO maybe through solve_position constraint
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

//first one should stay in place
void PBD_Simulation::solve_position_constraint(Particle *p1, Particle *p2) {

}

//distances should be l
void PBD_Simulation::solve_distance_constraint(Particle *p1, Particle *p2) {

    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - l) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = -d_p1;

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
