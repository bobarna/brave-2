#ifndef BRAVE2_SAVE_IMAGE_H
#define BRAVE2_SAVE_IMAGE_H


#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "other/stb_image_write.h"


void saveImage(const std::string &path, GLFWwindow *w) {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_bmp(path.c_str(), width, height, nrChannels, buffer.data());//, stride);
}


#endif //BRAVE2_SAVE_IMAGE_H
