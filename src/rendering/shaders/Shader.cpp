#include "Shader.h"

void Shader::setUniformMaterial(Material &material, const std::string &name) {
    setUniform(material.kd, name + ".kd");
    setUniform(material.ks, name + ".ks");
    setUniform(material.ka, name + ".ka");
    setUniform(material.shininess, name + ".shininess");
}

void Shader::setUniformLight(const Light &light, const std::string &name) {
    setUniform(light.La, name + ".La");
    setUniform(light.Le, name + ".Le");
    setUniform(light.wLightPos, name + ".wLightPos");
}