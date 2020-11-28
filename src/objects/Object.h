#ifndef BRAVE2_OBJECT_H
#define BRAVE2_OBJECT_H

#include "PBDSimulation.h"
#include "../utils/math.h"
#include "../rendering/Shader.h"
#include "Geometry.h"
#include "../rendering/RenderState.h"

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

class HairSimObject : public Object {
public:
    //TODO why can't I refactor this to .cpp file???
    HairSimObject(Shader *_shader, PBDSimulation *_sim) : Object(_shader, _sim) {};
    void ResetExternalForces();
    void AddForce(vec3 f);
    void Animate(float delta_t) override;
};

class HeadObject : public Object {
public:
    HeadObject(Shader *_shader, Geometry *_geometry, Material *_material, Texture *_texture);
};

#endif //BRAVE2_OBJECT_H
