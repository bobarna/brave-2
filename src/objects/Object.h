#ifndef BRAVE2_OBJECT_H
#define BRAVE2_OBJECT_H

#include "../utils/math.h"
#include "../geometries/Geometry.h"
#include "../rendering/shaders/Shader.h"

class Object {
protected:
    vec3 scale, translation, rotationAxis;
    float rotationAngle;

    Geometry *geometry;
    Shader *shader;
    Material *material;
    Texture *texture;
public:
    Object(Shader *_shader, Geometry *_sim, Material *_material = nullptr, Texture *_texture = nullptr);

    virtual void SetModelingTransform(mat4 &M, mat4 &Minv);

    void Draw(RenderState state);

    virtual void Animate(float delta_t);

    void Scale(vec3 s);

    void Translate(vec3 t);

    void Rotate(vec3 axis, float angle);
};


#endif //BRAVE2_OBJECT_H
