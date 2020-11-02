#include "HairSimulation.h"

void HairSimulation::add_force_to_all_sims(vec3 force) {
    external_forces += force;
    for (auto &sim : sims)
        sim.add_force(force);

    std::cout << "External forces:" << external_forces << std::endl;
}

HairSimulation::HairSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg) :
        head(_head),
        nr_sims(_nr_sims),
        nr_segments(_nr_segments),
        l_seg(_l_seg),
        external_forces(.0f, .0f, .0f) {
    // placing hair on the head
    propagateHead();
}

void HairSimulation::propagateHead() {
    srand(time(nullptr));
    float step = M_PI / nr_sims;
    float curr_angle = M_PI;
    float r = 0.20f;

    if (nr_sims == 1) r = 0.0;

    for (size_t i = 0; i < nr_sims; i++) {

        vec3 color = util::getRandomRGBColorAround(vec3(222.0f, 101.0f, 32.0f), vec3(40.0f, 20.0f, 20.0f));
        vec2 curr_pos(head.x + cosf(curr_angle) * r * 1.2, head.y + sinf(curr_angle) * r * 1.1);
//            vec2 curr_pos(head.x + cosf(curr_angle) * r, head.y + sinf(curr_angle) * r);

        float normalized_seg_size = l_seg + r * sinf(curr_angle) / nr_segments;
        float l_seg_rand = util::randomOffsetf(normalized_seg_size, l_seg * 0.2f);
//            float l_seg_rand = util::randomOffsetf(l_seg, 0.0f);

        sims.emplace_back(nr_segments, l_seg_rand, color, curr_pos);
        curr_angle -= step;
    }
}

void HairSimulation::update(float dt) {
    for (auto &sim : sims)
        sim.update(dt);
}

void HairSimulation::draw() {
    for (auto &sim : sims)
        sim.draw();
}

vec3 HairSimulation::get_external_forces() const {
    return external_forces;
}

HairSimulation::PBD_Strand::PBD_Strand(size_t n, float l, vec3 color, vec3 pos) :
        n(n), l(l), color(color), start_pos(pos) {
    initialize();
}

void HairSimulation::PBD_Strand::initialize() {
    coll.emplace_back(0.5,-1, 1);
    coll.emplace_back(0.5, 1, 0);
    coll.emplace_back(0.5,-1,-1);

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

void HairSimulation::PBD_Strand::add_force(vec3 f) {
    external_forces += f;
}

void HairSimulation::PBD_Strand::update(float dt) {
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

    // generate collision constraints

    // solve distance constraints
    // TODO find out ideal number of iterations
    size_t num_iter = 20;
    for (size_t iter = 0; iter < num_iter; iter++) {
        //keep first particle in place
        particles[0]->tmp_pos = particles[0]->pos;
        //distance between subsequent particles should be l
        for (size_t i = 1; i < particles.size(); i++) {
            solve_distance_constraint(particles[i - 1], particles[i], l);
            if(i < particles.size()-2) solve_bending_constraint(particles[i-1], particles[i+1], l*0.98);
            solve_collision_constraint(particles[i], coll[0], coll[1], coll[2]);
        }
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

void HairSimulation::PBD_Strand::draw() {
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

vec3 HairSimulation::PBD_Strand::get_external_forces() {
    return external_forces;
}

void HairSimulation::PBD_Strand::solve_distance_constraint(Particle *p1, Particle *p2, float dist) {
    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

    p1->tmp_pos += d_p1;
    p2->tmp_pos += d_p2;
}

void HairSimulation::PBD_Strand::solve_bending_constraint(Particle *p1, Particle *p2, float dist) {
    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

    p1->tmp_pos += 0.6 * d_p1;
    p2->tmp_pos += 0.6 * d_p2;
}

void HairSimulation::PBD_Strand::solve_collision_constraint(Particle *p, vec3 &q1, vec3 &q2, vec3 &q3) {
    vec3 n = normalize(cross(q1-q3, q1-q2));

    if(dot(p->tmp_pos - q1, n) < 0) return;

    vec3 d_p = -( (p->tmp_pos - q2)*n)*n;

    p->tmp_pos += d_p;
}

void HairSimulation::PBD_Strand::solve_particle_collision_constraint(Particle *p1, Particle *p2) {

}
