#include "Shader.h"

void Shader::setUniform(const mat4& mat, const std::string& name) {
    int location = getLocation(name);
    if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, mat);
}

void Shader::Bind(RenderState state) {
    use();        // make this program run
    setUniform(state.MVP, "MVP");
}

int Shader::getLocation(const std::string &name) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location < 0) printf("uniform %s cannot be set\n", name.c_str());
    return location;
}

