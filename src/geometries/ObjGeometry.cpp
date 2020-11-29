#include "ObjGeometry.h"

ObjGeometry::ObjGeometry(const std::string &objPath) :
        Geometry(),
        objReader(objPath, vtxData) {
    Initialize();
    for(auto d : vtxData)
        std::cout << d << std::endl;
}

void ObjGeometry::Initialize() {
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData)*vtxData.size(), &vtxData[0], GL_STATIC_DRAW);
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