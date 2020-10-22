//
// Created by bobarna on 2020. 10. 22..
//

#ifndef BRAVE2_HAIRSIMULATION_H
#define BRAVE2_HAIRSIMULATION_H


#include "utils/math.h"
#include "FTL_Simulation.h"
#include "utils/util.h"

class HairSimulation {
public:
    vec3 head;
    size_t nr_sims;
    size_t nr_segments;
    float l_seg;
    std::vector<FTL_Simulation> sims;
    vec3 external_forces;

    void add_force_to_all_sims(vec3 force) {
        for (auto & sim : sims)
            sim.add_force(force);
    }

    HairSimulation(vec3 _head, size_t _nr_sims, size_t _nr_segments, float _l_seg) : head(_head), nr_sims(_nr_sims),
                                                                                     nr_segments(_nr_segments),
                                                                                     l_seg(_l_seg) {
        propagateHead();
        add_force_to_all_sims(vec3(-0.05f, -0.05f, 0.0f));
    }

    void propagateHead() {
        srand(time(nullptr));
        float step = M_PI / nr_sims;
        float curr_angle = M_PI;
        float r = 0.20f;
        for (size_t i = 0; i < nr_sims; i++) {

            vec3 color = util::getRandomRGBColorAround(vec3(222.0f, 101.0f, 32.0f), vec3(40.0f, 20.0f, 20.0f));
            vec2 curr_pos(head.x + cosf(curr_angle) * r * 1.2, head.y + sinf(curr_angle) * r * 1.1);

            float l_seg_rand = util::randomOffsetf(l_seg, 0.03f);

            sims.emplace_back(nr_segments, l_seg_rand, color, curr_pos);
            curr_angle -= step;
        }
    }

    void update(float dt) {
        for (auto &sim : sims)
            sim.update(dt);
    }

    void draw() {
        for (auto &sim : sims)
            sim.draw();
    }

};


#endif //BRAVE2_HAIRSIMULATION_H
