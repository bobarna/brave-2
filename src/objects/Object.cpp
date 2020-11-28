#include "Object.h"

Object::Object(Shader *_shader, Geometry *_geometry, Material *_material, Texture *_texture) :
        scale(vec3(1, 1, 1)), translation(vec3(0, 0, 0)), rotationAxis(0, 0, 1), rotationAngle(0) {
    shader = _shader;
    geometry = _geometry;
    material = _material;
    texture = _texture;
}

void Object::SetModelingTransform(mat4 &M, mat4 &Minv) {
    M = scale_matrix(scale) * rotation_matrix(rotationAngle, rotationAxis) * translate_matrix(translation);
    Minv = translate_matrix(-translation) * rotation_matrix(-rotationAngle, rotationAxis) *
           scale_matrix(vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
}

void Object::Draw(RenderState state) {
    mat4 M, Minv;
    SetModelingTransform(M, Minv);
    state.M = M;
    state.Minv = Minv;
    state.MVP = state.M * state.V * state.P;
    state.material = material;
    state.texture = texture;
    shader->Bind(state);
    geometry->Draw();
}

void Object::Animate(float delta_t) {
}


void Object::Scale(vec3 s) {
    scale *= s;
}

void Object::Translate(vec3 t) {
    translation += t;
}

void Object::Rotate(vec3 axis, float angle) {
    rotationAxis = axis;
    rotationAngle = angle;
}

