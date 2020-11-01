#ifndef BRAVE2_OBJECT_H
#define BRAVE2_OBJECT_H


#include "utils/math.h"
#include "Geometry.h"
#include "RenderState.h"
#include "Shader.h"

class Object {
    vec3 scale, translation, rotationAxis;
    float rotationAngle;

    Shader *shader;
    Material *material;
    Texture *texture;
    Geometry *geometry;

public:
    Object(Shader *_shader, Material *_material, Texture *_texture, Geometry* _geometry);

    virtual void SetModelingTransform(mat4 &M, mat4 &Minv);

    void Draw(RenderState state);

    virtual void Animate(float tstart, float tend);

    void Scale(vec3 s);
    void Translate(vec3 t);
    void Rotate(vec3 axis, float angle);
};

#endif //BRAVE2_OBJECT_H
