#ifndef BRAVE2_HEADOBJECT_H
#define BRAVE2_HEADOBJECT_H

#include "Object.h"

class HeadObject : public Object {
public:
    HeadObject(Shader *_shader, Geometry *_geometry, Material *_material, Texture *_texture);

    VertexData GetVertexDataByUV(float u, float v);

    Geometry* getGeometry();
};


#endif //BRAVE2_HEADOBJECT_H
