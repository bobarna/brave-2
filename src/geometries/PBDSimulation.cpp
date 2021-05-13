#include <random>
#include "PBDSimulation.h"

void PBDSimulation::addForce(vec3 force) {
    externalForces += force;
}

PBDSimulation::PBDSimulation(size_t _nr_sims, size_t _nr_segments, float _l_seg) :
        nrStrands(_nr_sims),
        nrSegments(_nr_segments),
        lSeg(_l_seg),
        externalForces(.0f, .0f, .0f) {

    // placing the fibers of the cloth
    for (size_t i = 0; i < nrStrands; i++) {
        vec3 currPos(-0.15f, -0.15f, lSeg*0.5f * (float)i - 0.15);

        vec3 color = vec3(0.9f, 0.3f, 0.3f);
        strands.emplace_back(CreateFiber(nrSegments, lSeg, currPos * vec3(1, -1, 1), color));
    }

}

void PBDSimulation::update(float dt) {
    // setting velocity
    for (auto &strand : strands)
        for (auto &p : strand) {
            p->v = p->v + dt * (p->w) * externalForces;
            // damp velocities
            p->v *= .99f;

            // calculating temporal positions
            p->tmp_pos = p->pos + dt * p->v;
        }

    // TODO generate collision constraints

    // solve constraints
    size_t num_iter = 10;
    for (size_t iter = 0; iter < num_iter; iter++) {
        for (size_t s = 0; s < strands.size(); s++) {
            std::vector<Particle*>& strand = strands.at(s);

            // TODO position constraint
            //keep first particle in place
            strand.at(0)->tmp_pos = strand.at(0)->pos;
            // keep the last one fixed as well
            strand.at(strand.size()-1)->tmp_pos = strand.at(strand.size()-1)->pos;

            //distance between other particles should be l
            for (size_t i = 1; i < strand.size(); i++) {
                solve_distance_constraint(strand[i - 1], strand[i], lSeg);
            }

            // distance constraint for neighboring strands
            if(s != 0 && s != strands.size() - 1) {
                std::vector<Particle*>& next_strand = strands.at(s+1);
                // TODO refactor distance into constant
                for (size_t i = 1; i < strand.size()-1; i++)
                    solve_distance_constraint(strand[i], next_strand[i], lSeg);
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
    for (size_t curr_strand = 0; curr_strand < strands.size()-1; curr_strand ++) {
        auto strand = strands[curr_strand];
        auto next_strand = strands[curr_strand+1];
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

            particlePosAndColor.push_back(next_strand[i]->pos.x);
            particlePosAndColor.push_back(next_strand[i]->pos.y);
            particlePosAndColor.push_back(next_strand[i]->pos.z);
            particlePosAndColor.push_back(next_strand[i]->color.x);
            particlePosAndColor.push_back(next_strand[i]->color.y);
            particlePosAndColor.push_back(next_strand[i]->color.z);

            particlePosAndColor.push_back(next_strand[i]->pos.x);
            particlePosAndColor.push_back(next_strand[i]->pos.y);
            particlePosAndColor.push_back(next_strand[i]->pos.z);
            particlePosAndColor.push_back(next_strand[i]->color.x);
            particlePosAndColor.push_back(next_strand[i]->color.y);
            particlePosAndColor.push_back(next_strand[i]->color.z);

            particlePosAndColor.push_back(strand[i + 1]->pos.x);
            particlePosAndColor.push_back(strand[i + 1]->pos.y);
            particlePosAndColor.push_back(strand[i + 1]->pos.z);
            particlePosAndColor.push_back(strand[i + 1]->color.x);
            particlePosAndColor.push_back(strand[i + 1]->color.y);
            particlePosAndColor.push_back(strand[i + 1]->color.z);

            particlePosAndColor.push_back(next_strand[i+1]->pos.x);
            particlePosAndColor.push_back(next_strand[i+1]->pos.y);
            particlePosAndColor.push_back(next_strand[i+1]->pos.z);
            particlePosAndColor.push_back(next_strand[i+1]->color.x);
            particlePosAndColor.push_back(next_strand[i+1]->color.y);
            particlePosAndColor.push_back(next_strand[i+1]->color.z);
        }
    }


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // _STREAM_ indicates that the data will be modified, and then used only a few (one) time.
    // The buffer usage token might be changed to GL_DYNAMIC_DRAW when moving on to simulating positions on the GPU
    glBufferData(GL_ARRAY_BUFFER, particlePosAndColor.size() * sizeof(float), particlePosAndColor.data(),
                 GL_STREAM_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    // normal attribute
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    glLineWidth(1.0f);
    glDrawArrays(GL_TRIANGLES, 0, particlePosAndColor.size() / 6);
}

vec3 PBDSimulation::getExternalForces() const {
    return externalForces;
}

std::vector<Particle *> PBDSimulation::CreateFiber(size_t n, float l, vec3 startPos, vec3 color) {
    vec3 currPos = startPos;
    std::vector<Particle *> currentStrand;

    for (size_t i = 0; i < n; i++) {
        // random mass
        // A single strand of hair can weigh between .2 – .5 milligrams,
        // which is 2.0e-7 kg == 10^(-7) kg
        float m = util::randomOffsetf(.35f, .15f);

        // first and last particle's position is infinite
        if (i == 0 || i == n-1) currentStrand.push_back(new Particle(currPos, 0, color));
        else currentStrand.push_back(new Particle(currPos, 1 / m, color));

        // propagate particles horizontally
        currPos.x += l;
    }

    return currentStrand;
}

void PBDSimulation::resetExternalForces() {
    externalForces = vec3(0, 0, 0);
}
