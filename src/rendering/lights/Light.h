#ifndef BRAVE2_LIGHT_H
#define BRAVE2_LIGHT_H

#include "../../utils/math.h"

struct Light {
    vec3 La, Le;
    vec4 wLightPos; // homogenous coordinates, can be at ideal point

    void Animate(float t);
};

#endif //BRAVE2_LIGHT_H
