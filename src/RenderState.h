//
// Created by bobarna on 2020. 10. 31..
//

#ifndef BRAVE2_RENDERSTATE_H
#define BRAVE2_RENDERSTATE_H


#include "utils/math.h"
#include "MaterialAndTexture.h"
#include "Light.h"

struct RenderState {
    mat4 MVP, M, Minv, V, P;
    //TODO lights and other stuff
    Material* material;
    std::vector<Light> lights;
    Texture *texture;
    vec3 wEye;
};


#endif //BRAVE2_RENDERSTATE_H
