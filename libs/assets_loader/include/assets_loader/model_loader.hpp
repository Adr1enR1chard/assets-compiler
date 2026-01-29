#pragma once

#include <string>
#include <vector>
#include <assets_format/model_format.hpp>
#include <assets_format/mesh_format.hpp>

class ModelLoader
{
public:
    static bool LoadModel(const std::string &filePath, std::vector<std::vector<VertexLayout>> &meshesVertices, std::vector<std::vector<unsigned int>> &meshesIndices);
};