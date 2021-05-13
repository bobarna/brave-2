#include "Camera.h"

Camera::Camera(vec3 _wEye, vec3 _wLookAt, vec3 _wVup, int windowWidth, int windowHeight) {
    asp = (float) windowWidth / (float) windowHeight;
    fov = 75.0f * (float) M_PI / 180.0f;
    fp = 0.01f; //TODO is this right??
    bp = 10;

    wEye = _wEye;
    wLookAt = _wLookAt;
    wVup = _wVup;
}

mat4 Camera::V() const {
    vec3 w = normalize(wEye - wLookAt);
    vec3 u = normalize(cross(wVup, w));
    vec3 v = cross(w, u);
    return translate_matrix(-wEye) * mat4(
            u.x, v.x, w.x, 0,
            u.y, v.y, w.y, 0,
            u.z, v.z, w.z, 0,
            0, 0, 0, 1
    );
}

mat4 Camera::P() const {
    return mat4(1.0f / (tanf(fov / 2) * asp), 0, 0, 0,
                0, 1.0f / tanf(fov / 2), 0, 0,
                0, 0, -(fp + bp) / (bp - fp), -1,
                0, 0, -2 * fp * bp / (bp - fp), 0);
}

void Camera::Translate(vec3 dir) {
    vec3 forward = dir * vec3(0, 0, 1.f);

    wEye = normalize(wEye + dir) * length(wEye) + forward;
}

RenderState Camera::getState() {
    RenderState state;
    state.wEye = wEye;
    state.V = V();
    state.P = P();

    return state;
}
