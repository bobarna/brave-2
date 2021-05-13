#include "Scene.h"


Scene::Scene(int w, int h) : camera(vec3(0, 0.f, .5), // Camera position (wEye)
                                    vec3(0, 0, 0), // wLookat
                                    vec3(0, 1, 0), // wVup
                                    w, h)    {
}

void Scene::Build() {
    size_t nrSims = 100;
    size_t nrSegments = 50;
    float lSeg = 0.006f;

    // Lights
    lights.resize(3);
    lights[0].wLightPos = vec4(0.0f, 3.0f, 1.5f, 0);    // ideal point -> directional light source
    lights[0].La = vec3(0.0f, 7.0f, 7.0f);
    lights[0].Le = vec3(1, 1, 1);

    lights[1].wLightPos = vec4(2.0f, 3.0f, 1.0f, 0);    // ideal point -> directional light source
    lights[1].La = vec3(0.f, 3.5f, 3.5f);
    lights[1].Le = vec3(1, 1, 1);

    lights[2].wLightPos = vec4(0, 0, 1, 0);    // ideal point -> directional light source
    lights[2].La = vec3(0.f, 3.5f, 3.5f);
    lights[2].Le = vec3(1, 1, 1);

    Shader *basicShader = new BasicShader();
    basicShader->Bind(camera.getState());

    Shader *phongShader = new PhongShader();

    auto PBDSim = new PBDSimulation(nrSims, nrSegments, lSeg);
    auto PBDsimulationObject = new PBDSimulationObject(basicShader, PBDSim);
    PBDsims.push_back(PBDsimulationObject);

    auto SPHSim = new SPHSimulation();
    auto SPHsimulationObject = new SPHSimulationObject(basicShader, SPHSim);
    SPHsims.push_back(SPHsimulationObject);

}

void Scene::Render() {
    RenderState state = camera.getState();
    state.lights = lights;

    for (auto *so: PBDsims) so->Draw(state);
    for (auto *so : SPHsims) so->Draw(state);
    for (auto *o: objects) o->Draw(state);
}

//delta_t is infinitesimal
void Scene::Update(float delta_t) {
    // TODO if there was a keypress
    // Handle keypresses
    HandleKeyPress();

    for (Object *o: objects) o->Animate(delta_t);
    for (PBDSimulationObject *so : PBDsims) so->Animate(delta_t);
    for (SPHSimulationObject *so : SPHsims) so->Animate(delta_t);
}

void Scene::TranslateCamera(vec3 t) {
    camera.Translate(t);
}

void Scene::ResetExternalForces() {
    for (auto sim : PBDsims)
        sim->ResetExternalForces();
    if (gravityOn)
        for (auto sim : PBDsims)
            sim->AddForce({0, -GRAVITY_ABS_VALUE, 0});
}

void Scene::addForce(vec3 f) {
    for (auto sim : PBDsims)
        sim->AddForce(f);
}

void Scene::HandleKeyPress() {
    // G turns gravity on
    if (inputHandler->IsPressed(GLFW_KEY_G) && !inputHandler->IsAltPressed())
        gravityOn = true;
    // Alt+G turns gravity off
    if (inputHandler->IsPressed(GLFW_KEY_G) && inputHandler->IsAltPressed())
        gravityOn = false;
    if (inputHandler->IsPressed(GLFW_KEY_W) ||
        inputHandler->IsPressed(GLFW_KEY_A) ||
        inputHandler->IsPressed(GLFW_KEY_S) ||
        inputHandler->IsPressed(GLFW_KEY_D) ||
        inputHandler->IsPressed(GLFW_KEY_Q) ||
        inputHandler->IsPressed(GLFW_KEY_E))
        HandleCameraMove();
}

void Scene::HandleCameraMove() {
    vec3 vec = vec3(0, 0, 0);
    if (inputHandler->IsPressed(GLFW_KEY_W))
        vec = vec3(0, 0, -1);
    if (inputHandler->IsPressed(GLFW_KEY_A))
        vec = vec3(-1, 0, 0);
    if (inputHandler->IsPressed(GLFW_KEY_S))
        vec = vec3(0, 0, 1);
    if (inputHandler->IsPressed(GLFW_KEY_D))
        vec = vec3(1, 0, 0);
    if (inputHandler->IsPressed(GLFW_KEY_Q))
        vec = vec3(0, 1, 0);
    if (inputHandler->IsPressed(GLFW_KEY_E))
        vec = vec3(0, -1, 0);

    TranslateCamera(vec * 0.1);
}
