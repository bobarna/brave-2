#ifndef BRAVE2_RENDERSTATE_H
#define BRAVE2_RENDERSTATE_H

#include "utils/math.h"

struct RenderState {
    mat4 MVP, M, Minv, V, P;
    vec3 wEye;
};

#endif //BRAVE2_RENDERSTATE_H
