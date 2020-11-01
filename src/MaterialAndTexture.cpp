#include "MaterialAndTexture.h"

CheckerBoardTexture::CheckerBoardTexture(const int width, const int height) : Texture() {
    std::vector<vec4> image(width * height);
    const vec4 yellow(1, 1, 0, 1), blue(0, 0, 1, 1);
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            image[y * width + x] = (x & 1) ^ (y & 1) ? yellow : blue;
        }
    create(width, height, image, GL_NEAREST);
}
