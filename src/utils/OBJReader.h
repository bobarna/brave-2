#ifndef BRAVE2_OBJREADER_H
#define BRAVE2_OBJREADER_H

#include <string>
#include <vector>
#include <fstream>
#include "math.h"

class OBJReader {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<vec3> temp_vertices;
    std::vector<vec2> temp_uvs;
    std::vector<vec3> temp_normals;

    std::ifstream is;

public:
    OBJReader(const std::string &filePath, std::vector<vec3> &vertices, std::vector<vec2> &uvs,
              std::vector<vec3> &normals);

    void readData();

    void readVertexLine();

    void readUVLine();

    void readNormalLine();

    void readFaceLine();
};

struct chlit {
    char c_target;

    chlit(char _c) : c_target(_c) {}

    friend std::istream &operator>>(std::istream &is, chlit x) {
        char c;
        if (is >> c && c != x.c_target)
            is.setstate(std::iostream::failbit);
        return is;
    }

};

#endif //BRAVE2_OBJREADER_H
