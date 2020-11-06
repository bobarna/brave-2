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
    std::vector<float> particle_pos_and_color;
    for (auto &sim : sims)
        sim.getParticlesAsLines(particle_pos_and_color);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // _STREAM_ indicates that the data will be modified, and then used only a few (one) time.
    // The buffer usage token might be changed to GL_DYNAMIC_DRAW when moving on to simulating positions on the GPU
    glBufferData(GL_ARRAY_BUFFER, particle_pos_and_color.size()*sizeof(float), particle_pos_and_color.data(), GL_STREAM_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void *) (sizeof(float)*3));

    glLineWidth(1.0f);
    glDrawArrays(GL_LINES, 0, particle_pos_and_color.size()/6);
}

vec3 HairSimulation::get_external_forces() const {
    return external_forces;
}
