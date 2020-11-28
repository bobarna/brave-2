#ifndef BRAVE2_CAMERA_H
#define BRAVE2_CAMERA_H


#include "../utils/math.h"
#include "RenderState.h"

class Camera {
public:
    /// Position of the camera in world coordinates.
    vec3 wEye;

    /// Location of the center of the window.
    vec3 wLookAt;

    /** @brief
     * To find the vertical position of the window.
     * If wVup is not perpendicular to (wLookAt-wEye),
     * then only it's perpendicular position is used.
     */
    vec3 wVup;

    // Field of View - vertical size of the window
    float fov;

    // Aspect Ratio - vertical/horizontal
    float asp;

    // Front Clipping Plane
    float fp;

    // Back Clipping Plane
    float bp;

public:
    Camera(vec3 _wEye, vec3 _wLookAt, vec3 _wVup, int windowWidth, int windowHeight);

    // View Matrix: translates the center to the origin
    mat4 V() const;

    // Projection Matrix
    mat4 P() const;

    RenderState getState();

    // Translates the camera by dir
    void Translate(vec3 dir);

};


#endif //BRAVE2_CAMERA_H
