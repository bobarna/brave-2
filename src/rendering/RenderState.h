#ifndef BRAVE2_RENDERSTATE_H
#define BRAVE2_RENDERSTATE_H

#include "../utils/math.h"
#include "../utils/GPUProgram.h"
#include "materials/Material.h"
#include "lights/Light.h"

struct RenderState {
    mat4 MVP, M, Minv, V, P;
    vec3 wEye;
    Texture *texture;
    Material *material;

    std::vector<Light> lights;
};

#endif //BRAVE2_RENDERSTATE_H
