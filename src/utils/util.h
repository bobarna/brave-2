#ifndef BRAVE2_UTIL_H
#define BRAVE2_UTIL_H

#include "math.h"

namespace util {
    vec3 getRandomRGBColorAround(vec3 base, vec3 offset);

    float randomOffsetf(float base, float offset);

    /// random float in interval [from;to)
    float randomBetween(float from, float to);
}


#endif //BRAVE2_UTIL_H
