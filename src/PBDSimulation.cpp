#include "PBDSimulation.h"

void PBDSimulation::addForce(vec3 force) {
    externalForces += force;
}

PBDSimulation::PBDSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg) :
        head(_head),
        nrStrands(_nr_sims),
        nrSegments(_nr_segments),
        lSeg(_l_seg),
        externalForces(.0f, .0f, .0f) {
    // placing hair on the head
    propagateHead();

    // set up distance constraints on each strand of hair
    for (auto &strand : strands)
        for (size_t i = 0; i < strand.size() - 1; i++)
            constraints.push_back(new DistanceConstraint(strand[i], strand[i + 1], lSeg));

    // set up bending constraint on each strand of hair
//    for (auto &strand : strands)
//        for (size_t i = 1; i < strand.size() - 2; i++)
//            constraints.push_back(new DistanceConstraint(strand[i - 1], strand[i + 1], lSeg * 0.98f, 0.6f));


    // set up collision constraint
//    for (auto &strand: strands)
//        for (auto p : strand) {
//            constraints.push_back(new CollisionConstraint(p, 0.5f, -1, 1));
//            constraints.push_back(new CollisionConstraint(p, 0.5f, 1, 0));
//            constraints.push_back(new CollisionConstraint(p, 0.5f, -1, -1));
//        }
}

void PBDSimulation::propagateHead() {
    srand(time(nullptr));
    float step = M_PI / nrStrands;
    float currAngle = M_PI;
    float r = 0.20f;

    // if there is only one strand, then place it in the middle
    if (nrStrands == 1) r = 0.0;

    for (size_t i = 0; i < nrStrands; i++) {
        vec3 color = util::getRandomRGBColorAround(vec3(222.0f, 101.0f, 32.0f), vec3(40.0f, 20.0f, 20.0f));
        vec3 curr_pos(head.x + cosf(currAngle) * r * 1.2, head.y + sinf(currAngle) * r * 1.1, head.z);

        float normalized_seg_size = lSeg + r * sinf(currAngle) / nrSegments;
        float l_seg_rand = util::randomOffsetf(normalized_seg_size, lSeg * 0.2f);
//            float l_seg_rand = util::randomOffsetf(lSeg, 0.0f);


        strands.emplace_back(CreateStrand(nrSegments, l_seg_rand, curr_pos, color));
        currAngle -= step;
    }
}

void PBDSimulation::update(float dt) {
    // setting velocity
    for (auto &strand : strands)
        for (auto &p : strand) {
            p->v = p->v + dt * (p->w) * externalForces;
            p->v *= .99f;
            // calculating temporal positions
            p->tmp_pos = p->pos + dt * p->v;
        }

    // TODO generate collision constraints

    // solve constraints
    size_t num_iter = 5;
    for (size_t iter = 0; iter < num_iter; iter++) {
        for (auto &strand: strands) {
            //keep first particle in place
            // TODO position constraint
            strand.at(0)->tmp_pos = strand.at(0)->pos;
            for (auto &c : constraints)
                c->solve();
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

void PBDSimulation::draw() {
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
