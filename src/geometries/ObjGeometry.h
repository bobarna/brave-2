#ifndef BRAVE2_OBJGEOMETRY_H
#define BRAVE2_OBJGEOMETRY_H


#include "../utils/math.h"
#include "../utils/OBJReader.h"
#include "../objects/Object.h"

class ObjGeometry : public Geometry {
    std::string objPath;
    OBJReader objReader;
public:
    //TODO read every data from OBJ file
    ObjGeometry(const std::string &objPath);

    void Initialize();

    void Draw() override;
};


#endif //BRAVE2_OBJGEOMETRY_H
