#pragma once

#include <string>
#include <vector>
#include <assets_format/model_format.hpp>

class ModelLoader
{
public:
    static bool LoadModel(const std::string &filePath, std::vector<std::vector<VertexLayout>> &meshesVertices, std::vector<std::vector<unsigned int>> &meshesIndices, std::vector<MaterialDescriptor> &materials);

private:
    static bool LoadSubmesh(FILE *file, std::vector<VertexLayout> &vertices, std::vector<unsigned int> &indices);
    static bool LoadMaterial(FILE *file, MaterialDescriptor &material);
};