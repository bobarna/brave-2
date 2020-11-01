#include "Shader.h"

void Shader::setUniformLight(const Light &light, const std::string &name) {
    setUniform(light.La, name + ".La");
    setUniform(light.Le, name + ".Le");
    setUniform(light.wLightPos, name + ".wLightPos");
}

void Shader::setUniformMaterial(const Material &material, const std::string &name) {
    setUniform(material.kd, name + ".kd");
    setUniform(material.ks, name + ".ks");
    setUniform(material.ka, name + ".ka");
    setUniform(material.shininess, name + ".shininess");
}

void GouraudShader::Bind(RenderState state) {
    Use();        // make this program run
    setUniform(state.MVP, "MVP");
    setUniform(state.M, "M");
    setUniform(state.Minv, "Minv");
    setUniform(state.wEye, "wEye");
    setUniformMaterial(*state.material, "material");

    setUniform((int) state.lights.size(), "nLights");
    for (unsigned int i = 0; i < state.lights.size(); i++) {
        setUniformLight(state.lights[i], std::string("lights[") + std::to_string(i) + std::string("]"));
    }
}

GouraudShader::GouraudShader() {
    create(vertexSource, fragmentSource, "fragmentColor");
}


PhongShader::PhongShader() {
    create(vertexSource, fragmentSource, "fragmentColor");
}

void PhongShader::Bind(RenderState state) {
    Use();        // make this program run
    setUniform(state.MVP, "MVP");
    setUniform(state.M, "M");
    setUniform(state.Minv, "Minv");
    setUniform(state.wEye, "wEye");
    setUniform(*state.texture, std::string("diffuseTexture"));
    setUniformMaterial(*state.material, "material");

    setUniform((int) state.lights.size(), "nLights");
    for (unsigned int i = 0; i < state.lights.size(); i++) {
        setUniformLight(state.lights[i], std::string("lights[") + std::to_string(i) + std::string("]"));
    }
}

NPRShader::NPRShader() {
    create(vertexSource, fragmentSource, "fragmentColor");
}

void NPRShader::Bind(RenderState state) {
    Use();        // make this program run
    setUniform(state.MVP, "MVP");
    setUniform(state.M, "M");
    setUniform(state.Minv, "Minv");
    setUniform(state.wEye, "wEye");
    setUniform(*state.texture, std::string("diffuseTexture"));
    setUniform(state.lights[0].wLightPos, "wLightPos");
}
