#include "ObjGeometry.h"

ObjGeometry::ObjGeometry(const std::string &objPath) :
        Geometry(),
        objReader(objPath, vtxData) {
    Initialize();
}

void ObjGeometry::Initialize() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vtxData.size(), &vtxData[0], GL_STATIC_DRAW);
    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0);  // attribute array 0 = POSITION
    glEnableVertexAttribArray(1);  // attribute array 1 = NORMAL
    glEnableVertexAttribArray(2);  // attribute array 2 = TEXCOORD
    // attribute array, components/attribute, component type, normalize?, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) offsetof(VertexData, texcoord));
}

void ObjGeometry::Draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vtxData.size());
}

VertexData ObjGeometry::GetVertexDataByUV(float u, float v) {

    auto calc_a_b_c = [](vec2 p, vec2 t1, vec2 t2, vec2 t3) {
        float denom = (t2.y - t3.y) * (t1.x - t3.x) + (t3.x - t2.x) * (t1.y - t3.y);
        float a = ((t2.y - t3.y) * (p.x - t3.x) + (t3.x - t2.x) * (p.y - t3.y)) / denom;
        float b = ((t3.y - t1.y) * (p.x - t3.y) + (t1.x - t3.x) * (p.y - t3.y)) / denom;

        return vec3(a, b, 1 - a - b);
    };

    // iterates through all of the triangles of the mesh
    for (size_t i = 0; i < vtxData.size(); i += 3) {
        VertexData t1 = vtxData.at(i);
        VertexData t2 = vtxData.at(i + 1);
        VertexData t3 = vtxData.at(i + 2);

        // check if point is inside the triangle
        // this could be in a function, but I'm doing premature optimization - cause of all evil...
        // using baricentric method:
        // p = a*t1 + b*t2 + c*t3
        // p is inside triangle (t1, t2, t3) if and only if ( 0 <= a,b,c <= 1 )
        vec3 a_b_c{calc_a_b_c({u, v}, t1.texcoord, t2.texcoord, t3.texcoord)};

        // if point is inside the triangle
        if (0 <= a_b_c.x && a_b_c.x <= 1 &&
            0 <= a_b_c.y && a_b_c.y <= 1 &&
            0 <= a_b_c.z && a_b_c.z <= 1) {
            return {
                    a_b_c.x * t1.position + a_b_c.y * t2.position + a_b_c.z * t3.position,
                    a_b_c.x * t1.normal + a_b_c.y * t2.normal + a_b_c.z * t3.normal,
                    {u, v}
            };
        }
    }

    return {(VertexData) false};
}
