//
// Created by bobarna on 2020. 11. 28..
//

#include "BasicShader.h"

BasicShader::BasicShader() {
    create(vertexCode, fragmentCode, "fragmentColor");
}

void BasicShader::Bind(RenderState state) {
    Use(); //make this program run
    setUniform(state.MVP, "MVP");
}
