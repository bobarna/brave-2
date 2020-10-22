//
// Created by bobarna on 2020. 10. 22..
//

#ifndef BRAVE2_UTIL_H
#define BRAVE2_UTIL_H

#include "utils/math.h"

namespace util {
    vec3 getRandomRGBColorAround(vec3 base, vec3 offset) {
        float red = (float) rand() / RAND_MAX * offset.x + base.x;
        float green = (float) rand() / RAND_MAX * offset.y + base.y;
        float blue = (float) rand() / RAND_MAX * offset.z + base.z;
        red /= 255;
        green /= 255;
        blue /= 255;
        return vec3(red, green, blue);
    }

    float randomOffsetf(float base, float offset) {

    }
}


#endif //BRAVE2_UTIL_H
