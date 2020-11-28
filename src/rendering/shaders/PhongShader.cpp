#include "PhongShader.h"

void PhongShader::Bind(RenderState state) {
    Use();        // make this program run
    setUniform(state.MVP, "MVP");
    setUniform(state.M, "M");
    setUniform(state.Minv, "Minv");
    setUniform(state.wEye, "wEye");
    setUniform(*state.texture, std::string("diffuseTexture"));
    setUniformMaterial(*state.material, "materials");

    setUniform((int) state.lights.size(), "nLights");
    for (unsigned int i = 0; i < state.lights.size(); i++) {
        setUniformLight(state.lights[i], std::string("lights[") + std::to_string(i) + std::string("]"));
    }
}

PhongShader::PhongShader() {
    create(vertexSource, fragmentSource, "fragmentColor");
}
