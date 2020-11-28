#ifndef BRAVE2_BASICSHADER_H
#define BRAVE2_BASICSHADER_H

#include "Shader.h"

class BasicShader : public Shader {
    const char *vertexCode = R"(#version 330 core

                                 uniform mat4 MVP;

                                 layout (location = 0) in vec3 aPos;
                                 layout (location = 1) in vec3 aColor;

                                 out vec3 ourColor;

                                 void main()
                                 {
                                     gl_Position = vec4(aPos, 1.0) * MVP;
                                     ourColor = aColor;
                                 })";

    const char *fragmentCode = R"(#version 330 core
                                   out vec4 FragColor;

                                   in vec3 ourColor;

                                   void main()
                                   {
                                       gl_FragColor = vec4(ourColor, 1.0f);
                                   })";
public:
    BasicShader();

    void Bind(RenderState state) override;
};


#endif //BRAVE2_BASICSHADER_H
