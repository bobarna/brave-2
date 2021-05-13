#ifndef BRAVE2_OBJGEOMETRY_H
#define BRAVE2_OBJGEOMETRY_H


#include "../utils/math.h"
#include "../objects/Object.h"
#include "../utils/OBJReader.h"


// This class assumes that the mesh is made up entirely of triangles
// this is important for the GetVertexDataByUV function
class ObjGeometry : public Geometry {
    std::string objPath;
    OBJReader objReader;

public:
    //TODO read every data from OBJ file
    ObjGeometry(const std::string &objPath);

    void Initialize();

    void Draw() override;

    // Assumes that the mesh is made up of triangles
    VertexData GetVertexDataByUV(float u, float v) override;
};


#endif //BRAVE2_OBJGEOMETRY_H
