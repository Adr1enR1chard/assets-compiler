#pragma once

#include <string>
#include <vector>
#include <assets_format/model_format.hpp>

class ModelLoader
{
public:
    static bool LoadModel(const std::string &filePath, std::vector<MeshData> &meshes, std::vector<MaterialDescriptor> &materials);

private:
    static bool LoadSubmesh(FILE *file, MeshData &mesh);
    static bool LoadMaterial(FILE *file, MaterialDescriptor &material);
};