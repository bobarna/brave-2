#include "Scene.h"
#include "shaders/BasicShader.h"
#include "../geometries/ParamSurface.h"
#include "shaders/PhongShader.h"
#include "../utils/OBJReader.h"
#include "../geometries/ObjGeometry.h"

Scene::Scene(int w, int h) : camera(vec3(0, -.15f, .5), // Camera position (wEye)
                                    vec3(0, -.15f, 0), // wLookat
                                    vec3(0, 1, 0), // wVup
                                    w, h) {
}


void Scene::Build() {
    size_t nrSims = 200;
    size_t nrSegments = 30;
    float lSeg = 0.025f;


    Shader *basicShader = new BasicShader();
    basicShader->Bind(camera.getState());

    auto sphere = new Sphere();
    Shader *phongShader = new PhongShader();
    Material *headMaterial = new Material;
    headMaterial->kd = vec3(0.5f, 0.3f, .3f);
    headMaterial->ks = vec3(.1, .05, .05);
    headMaterial->ka = vec3(.2f, .2f, .2f);
    headMaterial->shininess = 20;

    Texture *headTexture = new UniformColorTexture(.6f, .2f, 0);
//    Texture *headTexture = new CheckerBoardTexture(1, 0);

    auto headObject = new HeadObject(phongShader, sphere, headMaterial, headTexture);

    headObject->Scale(vec3(.15, .1, .1));
    objects.push_back(headObject);

    auto PBDSim = new PBDSimulation(headObject, nrSims, nrSegments, lSeg);
    auto simulationObject = new HairSimObject(headObject, basicShader, PBDSim);
    sims.push_back(simulationObject);

    auto testObject =
            new Object(phongShader,
                       new ObjGeometry("../data/susanne.obj"),
                       headMaterial,
                       headTexture);

    objects.push_back(testObject);

    // Lights
    lights.resize(3);
    lights[0].wLightPos = vec4(0, 1, 0, 0);    // ideal point -> directional light source
    lights[0].La = vec3(0.1f, 0.1f, 1);
    lights[0].Le = vec3(1, 1, 1);

    lights[1].wLightPos = vec4(-1, .2, 1, 0);    // ideal point -> directional light source
    lights[1].La = vec3(0.2f, 0.2f, 0.2f);
    lights[1].Le = vec3(5, 3, 2);

    lights[2].wLightPos = vec4(0, 0, 1, 0);    // ideal point -> directional light source
    lights[2].La = vec3(0.1f, 0.1f, 0.1f);
    lights[2].Le = vec3(5, 3, 2);
}

void Scene::Render() {
    RenderState state = camera.getState();
    state.lights = lights;

    for (auto *so: sims) so->Draw(state);
    for (auto *o: objects) o->Draw(state);
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
