#include "PBDSimulation.h"

void PBDSimulation::addForce(vec3 force) {
    externalForces += force;
    for (auto &sim : sims)
        sim.add_force(force);

    std::cout << "External forces:" << externalForces << std::endl;
}

PBDSimulation::PBDSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg) :
        head(_head),
        nrSims(_nr_sims),
        nrSegments(_nr_segments),
        lSeg(_l_seg),
        externalForces(.0f, .0f, .0f) {
    // placing hair on the head
    propagateHead();
}

void PBDSimulation::propagateHead() {
    srand(time(nullptr));
    float step = M_PI / nrSims;
    float currAngle = M_PI;
    float r = 0.20f;

    if (nrSims == 1) r = 0.0;

    for (size_t i = 0; i < nrSims; i++) {

        vec3 color = util::getRandomRGBColorAround(vec3(222.0f, 101.0f, 32.0f), vec3(40.0f, 20.0f, 20.0f));
        vec2 curr_pos(head.x + cosf(currAngle) * r * 1.2, head.y + sinf(currAngle) * r * 1.1);
//            vec2 curr_pos(head.x + cosf(currAngle) * r, head.y + sinf(currAngle) * r);

        float normalized_seg_size = lSeg + r * sinf(currAngle) / nrSegments;
        float l_seg_rand = util::randomOffsetf(normalized_seg_size, lSeg * 0.2f);
//            float l_seg_rand = util::randomOffsetf(lSeg, 0.0f);

        sims.emplace_back(nrSegments, l_seg_rand, color, curr_pos);
        currAngle -= step;
    }
}

void PBDSimulation::update(float dt) {
    for (auto &sim : sims)
        sim.update(dt);
}

void PBDSimulation::draw() {
    std::vector<float> particlePosAndColor;
    for (auto &sim : sims)
        sim.getParticlesAsLines(particlePosAndColor);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // _STREAM_ indicates that the data will be modified, and then used only a few (one) time.
    // The buffer usage token might be changed to GL_DYNAMIC_DRAW when moving on to simulating positions on the GPU
    glBufferData(GL_ARRAY_BUFFER, particlePosAndColor.size() * sizeof(float), particlePosAndColor.data(), GL_STREAM_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void *) (sizeof(float)*3));

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, particlePosAndColor.size() / 6);
}

vec3 PBDSimulation::getExternalForces() const {
    return externalForces;
}
