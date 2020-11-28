#include "HeadObject.h"


HeadObject::HeadObject(Shader *_shader, Geometry *_geometry, Material *_material, Texture *_texture) :
        Object(_shader, _geometry, _material, _texture) {}

VertexData HeadObject::GetVertexDataByUV(float u, float v) {
    mat4 M, Minv;
    SetModelingTransform(M, Minv);
    VertexData vD{geometry->GetVertexDataByUV(u, v)};
    vec4 WP = vec4(vD.position.x, vD.position.y, vD.position.z, 1) * M;
    vec4 WN = vec4(vD.normal.x, vD.normal.y, vD.normal.z, 0) * M;
    vD.position = vec3(WP.x, WP.y, WP.z);
    vD.normal = vec3(WN.x, WN.y, WN.z);

    std::cout << vD << std::endl;
    return vD;
}


