#include "Scene.h"

Scene::Scene(int w, int h) : camera(vec3(0, -0.2f, 1), // Camera position (wEye)
                                    vec3(0, -.2f, 0), // wLookat
                                    vec3(0, 1, 0), // wVup
                                    w, h) {

}

void Scene::Build() {
    vec3 headCenter(0.0f, 0.0f, 0.0f);
    size_t nrSims = 200;
    size_t nrSegments = 30;
    float lSeg = 0.025f;

    auto PBDSim = new PBDSimulation(headCenter, nrSims, nrSegments, lSeg);

    auto basicShader = new Shader();
    basicShader->use();

    auto simulationObject = new HairSimObject(basicShader, PBDSim);

    sims.push_back(simulationObject);
}

void Scene::Render() {
    RenderState state = camera.getState();
    for(auto *o: objects) o->Draw(state);
    for(auto *so: sims) so->Draw(state);
}

void Scene::Update(float delta_t) {
    // TODO if there was a keypress
    // Handle keypresses
    HandleKeyPress();

    for(Object *o: objects) o->Animate(delta_t);
    for(HairSimObject *so : sims) so->Animate(delta_t);
}

void Scene::TranslateCamera(vec3 t) {
    camera.Translate(t);
}

void Scene::ResetExternalForces() {
    for(auto sim : sims)
        sim->ResetExternalForces();
    if(gravityOn)
        for(auto sim : sims)
            sim->AddForce({0, -GRAVITY_ABS_VALUE, 0});
}

void Scene::addForce(vec3 f) {
    for(auto sim : sims)
        sim->AddForce(f);
}

void Scene::HandleKeyPress() {
    // G turns gravity on
    if(inputHandler->IsPressed(GLFW_KEY_G) && !inputHandler->IsAltPressed())
        gravityOn = true;
    // Alt+G turns gravity off
    if(inputHandler->IsPressed(GLFW_KEY_G) && inputHandler->IsAltPressed())
        gravityOn = false;
    if(inputHandler->IsPressed(GLFW_KEY_W) ||
       inputHandler->IsPressed(GLFW_KEY_A) ||
       inputHandler->IsPressed(GLFW_KEY_S) ||
       inputHandler->IsPressed(GLFW_KEY_D) ||
       inputHandler->IsPressed(GLFW_KEY_Q) ||
       inputHandler->IsPressed(GLFW_KEY_E))
            HandleCameraMove();
}

void Scene::HandleCameraMove() {
    vec3 vec = vec3(0, 0, 0);
    if(inputHandler->IsPressed(GLFW_KEY_W))
        vec = vec3(0, 0, -1);
    if(inputHandler->IsPressed(GLFW_KEY_A))
        vec = vec3(-1, 0, 0);
    if(inputHandler->IsPressed(GLFW_KEY_S))
        vec = vec3(0, 0, 1);
    if(inputHandler->IsPressed(GLFW_KEY_D))
        vec = vec3(1, 0, 0);
    if(inputHandler->IsPressed(GLFW_KEY_Q))
        vec = vec3(0, 1, 0);
    if(inputHandler->IsPressed(GLFW_KEY_E))
        vec = vec3(0, -1, 0);

    TranslateCamera(vec* 0.1);
}
