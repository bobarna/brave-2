#include "Scene.h"
#include "Geometry.h"

Scene::Scene(int w, int h):
    camera(vec3(0, 0, 8), // Camera position (wEye)
           vec3(0, 0, 0), // wLookat
           vec3(0, 1, 0), // wVup
           w, h)        // window dimensions
{
    std::cout << "camera is at position " << camera.wEye << ", looking at: " << camera.wLookAt << std::endl;
}

void Scene::Build() {
    // Shader
    Shader *phongShader = new PhongShader();
    Shader *gouraudShader = new GouraudShader();
    Shader *nprShader = new NPRShader();

    // Materials
    Material *material0 = new Material;
    material0->kd = vec3(0.6f, 0.4f, 0.2f);
    material0->ks = vec3(4, 4, 4);
    material0->ka = vec3(0.1f, 0.1f, 0.1f);
    material0->shininess = 100;

    Material *material1 = new Material;
    material1->kd = vec3(0.8f, 0.6f, 0.4f);
    material1->ks = vec3(0.3f, 0.3f, 0.3f);
    material1->ka = vec3(0.2f, 0.2f, 0.2f);
    material1->shininess = 30;

    // Textures
    Texture *texture4x8 = new CheckerBoardTexture(4, 8);
    Texture *texture15x20 = new CheckerBoardTexture(15, 20);

    // Geometries
    Geometry *sphere = new Sphere();

    // Create objects by setting up their vertex data on the GPU
    Object *sphereObject = new Object(phongShader, material0, texture15x20, sphere);
    sphereObject->Translate(vec3(-3, 3, 0));
//    sphereObject->Rotate(vec3(0,1,1), 0);
    sphereObject->Scale(vec3(2.0f, 1.1f, 1));
    objects.push_back(sphereObject);

    Object *sphereObject1 = new Object(gouraudShader, material0, texture15x20, sphere);
    sphereObject1->Translate(vec3(1,0,0));
    objects.push_back(sphereObject1);
    sphereObject1->Scale(vec3(10.0f, 10.0f, 10.0f));

    // Camera is already set up

    // Lights
    lights.resize(3);
    lights[0].wLightPos = vec4(5, 5, 4, 0);    // ideal point -> directional light source
    lights[0].La = vec3(0.1f, 0.1f, 1);
    lights[0].Le = vec3(3, 0, 0);

    lights[1].wLightPos = vec4(5, 10, 20, 0);    // ideal point -> directional light source
    lights[1].La = vec3(0.2f, 0.2f, 0.2f);
    lights[1].Le = vec3(0, 3, 0);

    lights[2].wLightPos = vec4(-5, 5, 5, 0);    // ideal point -> directional light source
    lights[2].La = vec3(0.1f, 0.1f, 0.1f);
    lights[2].Le = vec3(0, 0, 3);

}

void Scene::Render() {
    RenderState state = camera.getState();
    state.lights = lights;

    for(Object *o: objects) o->Draw(state);
}

void Scene::Animate(float tstart, float tend) {
//    camera.Animate(tend);
    for (auto & light : lights)
        light.Animate(tend);

    for(Object *o: objects) o->Animate(tstart, tend);
}

