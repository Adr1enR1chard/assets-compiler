#pragma once

#include <string>
#include <vector>
#include <assets_format/mesh_format.hpp>

class MeshLoader
{
public:
    static bool LoadMesh(const std::string &filePath, std::vector<VertexLayout> &vertices, std::vector<unsigned int> &indices);
};