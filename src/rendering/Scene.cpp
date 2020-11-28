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

    Shader *basicShader = new BasicShader();
    basicShader->Bind(camera.getState());

    auto simulationObject = new HairSimObject(basicShader, PBDSim);

    sims.push_back(simulationObject);

    auto sphere = new Sphere();
    Shader *phongShader = new PhongShader();
    Material *headMaterial = new Material;
    headMaterial->kd = vec3(0.6f, 0.2f, .2f);
    headMaterial->ks = vec3(.3, .2, .2);
    headMaterial->ka = vec3(.2f, .2f, .2f);
    headMaterial->shininess = 100;

//    Texture *headTexture = new UniformColorMaterial(.6f, .2f, 0);
    Texture *headTexture = new CheckerBoardTexture(10, 10);

    auto headObject = new HeadObject(phongShader, sphere, headMaterial, headTexture);

    headObject->Scale(vec3(.35, .3, .3));
    objects.push_back(headObject);

    // Lights
    lights.resize(3);
    lights[0].wLightPos = vec4(0, 1, 0, 0);    // ideal point -> directional light source
    lights[0].La = vec3(0.1f, 0.1f, 1);
    lights[0].Le = vec3(1, 1, 1);

    lights[1].wLightPos = vec4(-1, .2, -1, 0);    // ideal point -> directional light source
    lights[1].La = vec3(0.2f, 0.2f, 0.2f);
    lights[1].Le = vec3(5, 3, 2);

    lights[2].wLightPos = vec4(0, 0, -1, 0);    // ideal point -> directional light source
    lights[2].La = vec3(0.1f, 0.1f, 0.1f);
    lights[2].Le = vec3(5, 3, 2);
}

void Scene::Render() {
    RenderState state = camera.getState();
    state.lights = lights;
    for (auto *o: objects) o->Draw(state);
    for (auto *so: sims) so->Draw(state);
}

//delta_t is infinitesimal
void Scene::Update(float delta_t) {
    // TODO if there was a keypress
    // Handle keypresses
    HandleKeyPress();

    for (Object *o: objects) o->Animate(delta_t);
    for (HairSimObject *so : sims) so->Animate(delta_t);
}

void Scene::TranslateCamera(vec3 t) {
    camera.Translate(t);
}

void Scene::ResetExternalForces() {
    for (auto sim : sims)
        sim->ResetExternalForces();
    if (gravityOn)
        for (auto sim : sims)
            sim->AddForce({0, -GRAVITY_ABS_VALUE, 0});
}

void Scene::addForce(vec3 f) {
    for (auto sim : sims)
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
