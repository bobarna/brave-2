#include "OBJReader.h"

OBJReader::OBJReader(const std::string &filePath, std::vector<VertexData> &out_vtxData) : is(filePath) {

    if (!is) {
        std::cerr << "Can't open OBJ file: " << filePath << std::endl;
        return;
    }

    readData();

    for (auto curr: out_vtxData)
        std::cout << curr << std::endl;
    for (unsigned int i : vertexIndices)
        out_vtxData.emplace_back(temp_vertices[i - 1], temp_normals[i - 1], temp_uvs[i - 1]);

}

void OBJReader::readData() {
    std::string lineType;

    // read the file word by word
    // the >> operator will only extract what can be considered a word from the stream,
    // using whitespaces as separators
    while (is >> lineType) {
        if (lineType == "#")
            // Comment, skip the line
            std::getline(is, lineType);
        else if (lineType == "v")
            readVertexLine();
        else if (lineType == "vt")
            readUVLine();
        else if (lineType == "vn")
            readNormalLine();
        else if (lineType == "f")
            readFaceLine();
        else {
            // ignore everything else
            std::cerr << "ignored line: " << lineType;
            // get rest of the line
            std::getline(is, lineType);
            // output rest of the line
            std::cerr << " " << lineType << std::endl;
        }
    }
}

void OBJReader::readVertexLine() {
    // Vertex
    vec3 v;
    is >> v.x >> v.y >> v.z;
    temp_vertices.push_back(v);
}

void OBJReader::readUVLine() {
    // UV coordinates
    vec2 uv;
    is >> uv.x >> uv.y;
    temp_uvs.push_back(uv);
}

void OBJReader::readNormalLine() {
    // Normal vector
    vec3 n;
    is >> n.x >> n.y >> n.z;
    temp_normals.push_back(n);
}

void OBJReader::readFaceLine() {
// Face
    std::string v1, v2, v3;
    unsigned int v[3], uv[3], n[3];
    is >> v[0] >> chlit('/') >> uv[0] >> chlit('/') >> n[0];
    is >> v[1] >> chlit('/') >> uv[1] >> chlit('/') >> n[1];
    is >> v[2] >> chlit('/') >> uv[2] >> chlit('/') >> n[2];
    if (is.fail()) {
        std::cerr << "can't read face data from OBJ file" << std::endl;
        throw std::exception();
    }
    vertexIndices.push_back(v[0]);
    vertexIndices.push_back(v[1]);
    vertexIndices.push_back(v[2]);
    uvIndices.push_back(uv[0]);
    uvIndices.push_back(uv[1]);
    uvIndices.push_back(uv[2]);
    normalIndices.push_back(n[0]);
    normalIndices.push_back(n[1]);
    normalIndices.push_back(n[2]);
}
