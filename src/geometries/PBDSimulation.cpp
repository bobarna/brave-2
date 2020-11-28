#include "PBDSimulation.h"

void PBDSimulation::addForce(vec3 force) {
    externalForces += force;
}

PBDSimulation::PBDSimulation(HeadObject* _head, size_t _nr_sims, size_t _nr_segments, float _l_seg) :
        nrStrands(_nr_sims),
        nrSegments(_nr_segments),
        lSeg(_l_seg),
        externalForces(.0f, .0f, .0f) {

    head = _head;
    // placing hair on the head
    propagateHead();

    collisionTriangles.emplace_back(0.5, -1, 1);
    collisionTriangles.emplace_back(0.5, 1, 0);
    collisionTriangles.emplace_back(0.5, -1, -1);
}

void PBDSimulation::propagateHead() {
    vec3 origin(0,0,0);
    for (size_t i = 0; i < nrStrands; i++) {
        vec3 color = util::getRandomRGBColorAround(vec3(222.0f, 101.0f, 32.0f), vec3(40.0f, 20.0f, 20.0f));
        strands.emplace_back(CreateStrand(nrSegments, lSeg, origin, color));
    }
}

void PBDSimulation::update(float dt) {
    // setting velocity
    for (auto &strand : strands)
        for (auto &p : strand) {
            p->v = p->v + dt * (p->w) * externalForces;
            // damp velocities
            // TODO better damping technique: (3.5) https://matthias-research.github.io/pages/publications/posBasedDyn.pdf
            p->v *= .99f;

            // calculating temporal positions
            p->tmp_pos = p->pos + dt * p->v;
        }

    // TODO generate collision constraints

    // solve constraints
    size_t num_iter = 10;
    for (size_t iter = 0; iter < num_iter; iter++) {
        for (auto &strand: strands) {
            //keep first particle in place
            // TODO position constraint
            strand.at(0)->tmp_pos = strand.at(0)->pos;
            //distance between subsequent particles should be l
            for (size_t i = 1; i < strand.size(); i++) {
                solve_distance_constraint(strand[i - 1], strand[i], lSeg);
                if (i < strand.size() - 1) solve_bending_constraint(strand[i - 1], strand[i + 1], lSeg * 0.9f);
                if (i < strand.size() - 2 && i > 1)
                    solve_bending_constraint(strand[i - 2], strand[i + 2], lSeg * 1.9f);
                solve_collision_constraint(strand[i],
                                           collisionTriangles[0], collisionTriangles[1], collisionTriangles[2]);
            }
        }
    }

    // updating velocities and positions
    // skipping first one
    // it stays in place
    for (auto &strand: strands)
        for (auto it = ++strand.begin(); it != strand.end(); ++it) {
            Particle *p = *it;
            p->v = (p->tmp_pos - p->pos) / dt;
            p->pos = p->tmp_pos;
        }
}

void PBDSimulation::solve_distance_constraint(Particle *p1, Particle *p2, float dist) {
    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

    p1->tmp_pos += d_p1;
    p2->tmp_pos += d_p2;
}

void PBDSimulation::solve_bending_constraint(Particle *p1, Particle *p2, float dist) {
    vec3 d_p1 = -(p1->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);
    vec3 d_p2 = (p2->w / (p1->w + p2->w)) *
                (length(p1->tmp_pos - p2->tmp_pos) - dist) *
                (p1->tmp_pos - p2->tmp_pos) / length(p1->tmp_pos - p2->tmp_pos);

    p1->tmp_pos += 0.6 * d_p1;
    p2->tmp_pos += 0.6 * d_p2;
}

void PBDSimulation::solve_collision_constraint(Particle *p, vec3 &q1, vec3 &q2, vec3 &q3) {
    vec3 n = normalize(cross(q1 - q3, q1 - q2));

    if (dot(p->tmp_pos - q1, n) < 0) return;

    vec3 d_p = -((p->tmp_pos - q2) * n) * n;

    p->tmp_pos += d_p;
}

void PBDSimulation::Draw() {
    std::vector<float> particlePosAndColor;
    for (auto &strand : strands)
        for (size_t i = 0; i < strand.size() - 1; ++i) {
            particlePosAndColor.push_back(strand[i]->pos.x);
            particlePosAndColor.push_back(strand[i]->pos.y);
            particlePosAndColor.push_back(strand[i]->pos.z);
            particlePosAndColor.push_back(strand[i]->color.x);
            particlePosAndColor.push_back(strand[i]->color.y);
            particlePosAndColor.push_back(strand[i]->color.z);

            particlePosAndColor.push_back(strand[i + 1]->pos.x);
            particlePosAndColor.push_back(strand[i + 1]->pos.y);
            particlePosAndColor.push_back(strand[i + 1]->pos.z);
            particlePosAndColor.push_back(strand[i + 1]->color.x);
            particlePosAndColor.push_back(strand[i + 1]->color.y);
            particlePosAndColor.push_back(strand[i + 1]->color.z);
        }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // _STREAM_ indicates that the data will be modified, and then used only a few (one) time.
    // The buffer usage token might be changed to GL_DYNAMIC_DRAW when moving on to simulating positions on the GPU
    glBufferData(GL_ARRAY_BUFFER, particlePosAndColor.size() * sizeof(float), particlePosAndColor.data(),
                 GL_STREAM_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) 0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, particlePosAndColor.size() / 6);
}

vec3 PBDSimulation::getExternalForces() const {
    return externalForces;
}

std::vector<Particle *> PBDSimulation::CreateStrand(size_t n, float l, vec3 startPos, vec3 color) {
    vec3 currPos = startPos;
    std::vector<Particle *> currentStrand;

    for (size_t i = 0; i < n; i++) {
        // random mass
        // A single strand of hair can weigh between .2 – .5 milligrams,
        // which is 2.0e-7 kg == 10^(-7) kg
        float m = util::randomOffsetf(.35f, .15f);

        // first particle's position is infinite
        if (i == 0) currentStrand.push_back(new Particle(currPos, 0, color));
        else currentStrand.push_back(new Particle(currPos, 1 / m, color));

        // propagate particles downward
        currPos.y -= l;
    }

    return currentStrand;
}

void PBDSimulation::resetExternalForces() {
    externalForces = vec3(0,0,0);
}
