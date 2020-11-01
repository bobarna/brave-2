#ifndef BRAVE2_MATERIALANDTEXTURE_H
#define BRAVE2_MATERIALANDTEXTURE_H

#include "utils/math.h"
#include "utils/gl_helper.h"

struct Material {
    vec3 kd, ks, ka;
    float shininess;
};


class CheckerBoardTexture : public Texture {
public:
    CheckerBoardTexture(int width = 0, int height = 0);
};


#endif //BRAVE2_MATERIALANDTEXTURE_H
