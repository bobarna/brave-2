#ifndef BRAVE2_SCENE_H
#define BRAVE2_SCENE_H


#include <vector>
#include "Camera.h"
#include "Object.h"

class Scene {
    std::vector<Object *> objects;
    std::vector<HairSimObject *> sims;
    Camera camera;

public:
    Scene(int w, int h);

    void Build();

    void Render();

    void Update(float delta_t);

    void TranslateCamera(vec3 t);

    void ResetExternalForces();

    void addForce(vec3 vec3);
};


#endif //BRAVE2_SCENE_H
