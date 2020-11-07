#ifndef BRAVE2_SHADER_H
#define BRAVE2_SHADER_H


#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "utils/math.h"

struct Shader {
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader()
    {
        // 1. retrieve the vertex/fragment source code from filePath
        const char* vertexCode = R"(#version 330 core
                                 
                                 uniform mat4 MVP;
                                 
                                 layout (location = 0) in vec3 aPos;
                                 layout (location = 1) in vec3 aColor;
                                 
                                 out vec3 ourColor;
                                 
                                 void main()
                                 {
                                     gl_Position = vec4(aPos, 1.0) * MVP;
                                     ourColor = aColor;
                                 })";

        const char* fragmentCode = R"(#version 330 core
                                   out vec4 FragColor;
                                   
                                   in vec3 ourColor;
                                   
                                   void main()
                                   {
                                       gl_FragColor = vec4(ourColor, 1.0f);
                                   })";

        const char* vShaderCode = vertexCode;
        const char * fShaderCode = fragmentCode;
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);


    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};


#endif //BRAVE2_SHADER_H
