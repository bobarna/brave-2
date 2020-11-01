#ifndef BRAVE2_SCENE_H
#define BRAVE2_SCENE_H


#include <vector>
#include "Camera.h"
#include "Object.h"

class Scene {
    std::vector<Object *> objects;
    Camera camera;
    std::vector<Light> lights;

public:
    Scene(int w, int h);

    void Build();

    void Render();

    void Animate(float tstart, float tend);

};


#endif //BRAVE2_SCENE_H
