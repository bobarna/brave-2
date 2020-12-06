#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>
#include "util.h"

vec3 util::getRandomRGBColorAround(vec3 base, vec3 offset) {
    float red = (float) rand() / RAND_MAX * offset.x + base.x;
    float green = (float) rand() / RAND_MAX * offset.y + base.y;
    float blue = (float) rand() / RAND_MAX * offset.z + base.z;
    red /= 255;
    green /= 255;
    blue /= 255;
    return vec3(red, green, blue);
}

float util::randomOffsetf(float base, float offset) {
    return (float) rand() / RAND_MAX * offset + base;
}

float util::randomBetween(float from, float to) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(from, to);
    return dis(gen);
}