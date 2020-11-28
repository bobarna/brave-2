#ifndef BRAVE2_MATERIAL_H
#define BRAVE2_MATERIAL_H


#include "../../utils/math.h"

struct Material {
    vec3 kd, ks, ka;
    float shininess;
};

#endif //BRAVE2_MATERIAL_H
