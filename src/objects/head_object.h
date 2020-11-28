#ifndef BRAVE2_HEAD_OBJECT_H
#define BRAVE2_HEAD_OBJECT_H


#include "Object.h"

class HeadObject : public Object {
public:
    HeadObject(Shader *_shader, Geometry *_geometry, Material *_material, Texture *_texture);
};


#endif //BRAVE2_HEAD_OBJECT_H
