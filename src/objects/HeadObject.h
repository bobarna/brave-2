#ifndef BRAVE2_HEADOBJECT_H
#define BRAVE2_HEADOBJECT_H

#include "Object.h"
#include "../geometries/ObjGeometry.h"

class HeadObject : public Object {
public:
    HeadObject(Shader *_shader, ObjGeometry *_geometry, Material *_material, Texture *_texture);

    VertexData GetVertexDataByUV(float u, float v);

    ObjGeometry* getGeometry();
};


#endif //BRAVE2_HEADOBJECT_H
