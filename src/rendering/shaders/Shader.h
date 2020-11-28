#ifndef BRAVE2_SHADER_H
#define BRAVE2_SHADER_H

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../utils/math.h"
#include "../RenderState.h"
#include "../../utils/GPUProgram.h"
#include "../materials/Material.h"
#include "../lights/Light.h"

struct Shader : public GPUProgram {
public:
    virtual void Bind(RenderState state) = 0;

    void setUniformMaterial(Material &material, const std::string &name);

    void setUniformLight(const Light &light, const std::string &name);
};


#endif //BRAVE2_SHADER_H
