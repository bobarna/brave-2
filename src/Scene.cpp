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

    PBDSimulation *PBDSim = new PBDSimulation(headCenter, nrSims, nrSegments, lSeg);

    Shader *basicShader = new Shader();
    basicShader->use();

    HairSimObject *simulationObject = new HairSimObject(basicShader, PBDSim);

    sims.push_back(simulationObject);
}

void Scene::Render() {
    RenderState state = camera.getState();
    for(auto *o: objects) o->Draw(state);
    for(auto *so: sims) so->Draw(state);
}

void Scene::Update(float delta_t) {
    for(Object *o: objects) o->Animate(delta_t);
    for(HairSimObject *so : sims) so->Animate(delta_t);
}

void Scene::TranslateCamera(vec3 t) {
    camera.wEye += t;
}

void Scene::ResetExternalForces() {
    for(auto sim : sims)
        sim->ResetExternalForces();
}

void Scene::addForce(vec3 f) {
    for(auto sim : sims)
        sim->AddForce(f);
}
