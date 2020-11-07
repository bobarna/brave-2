#include "HairSimulationObject.h"

HairSimulationObject::HairSimulationObject(Shader *_shader, HairSimulation *_sim) :
        scale(vec3(1, 1, 1)), translation(vec3(0, 0, 0)), rotationAxis(0, 0, 1), rotationAngle(0) {
    shader = _shader;
    sim = _sim;
}

void HairSimulationObject::SetModelingTransform(mat4 &M, mat4 &Minv) {
    M = scale_matrix(scale) * rotation_matrix(rotationAngle, rotationAxis) * translate_matrix(translation);
    Minv = translate_matrix(-translation) * rotation_matrix(-rotationAngle, rotationAxis) *
           scale_matrix(vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
}

void HairSimulationObject::Draw(RenderState state) {
    mat4 M, Minv;
    SetModelingTransform(M, Minv);
    state.M = M;
    state.Minv = Minv;
    state.MVP = state.M * state.V * state.P;
//    TODO shader->Bind(state);
    sim->draw();
}

void HairSimulationObject::Animate(float delta_t) {

}

void HairSimulationObject::Scale(vec3 s) {
    scale *= s;
}

void HairSimulationObject::Translate(vec3 t) {
    translation += t;
}

void HairSimulationObject::Rotate(vec3 axis, float angle) {
    rotationAxis = axis;
    rotationAngle = angle;
}
