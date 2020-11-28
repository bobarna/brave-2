#ifndef BRAVE2_SCENE_H
#define BRAVE2_SCENE_H

#include <vector>
#include "../geometries/PBDSimulation.h"
#include "Camera.h"
#include "../objects/Object.h"
#include "../Constants.h"
#include "../InputHandler.h"
#include "../objects/HairSimObject.h"

#include <GLFW/glfw3.h>

class Scene {
    std::vector<Object *> objects;
    std::vector<HairSimObject *> sims;
    Camera camera;
    std::vector<Light> lights;

    InputHandler* inputHandler = InputHandler::GetInstance();

    bool gravityOn = true;

public:
    Scene(int w, int h);

    void Build();
    void Update(float delta_t);
    void Render();

    void ResetExternalForces();
    void addForce(vec3 vec3);

    void HandleKeyPress();

    void HandleCameraMove();
    void TranslateCamera(vec3 t);
};


#endif //BRAVE2_SCENE_H
