#include <assets_loader/model_loader.hpp>
#include <assets_loader/mesh_loader.hpp>
#include <assets_format/model_format.hpp>
#include <assets_format/format_validator.hpp>

#include <iostream>

bool ModelLoader::LoadModel(const std::string &filePath, std::vector<std::vector<VertexLayout>> &meshesVertices, std::vector<std::vector<unsigned int>> &meshesIndices)
{
    std::string modelPath = get_model_path(filePath);
    FILE *file = fopen(modelPath.c_str(), "rb");
    if (!file)
    {
        std::cerr << "Failed to open model file: " << filePath << std::endl;
        return false;
    }

    ModelHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsModelHeaderValid(header))
    {
        std::cerr << "Invalid model format: " << filePath << std::endl;
        fclose(file);
        return false;
    }

    meshesVertices.resize(header.meshCount);
    meshesIndices.resize(header.meshCount);

    for (uint32_t i = 0; i < header.meshCount; ++i)
    {
        std::vector<VertexLayout> vertices;
        std::vector<unsigned int> indices;
        if (!MeshLoader::LoadMesh(get_submesh_path(modelPath, i), vertices, indices))
        {
            std::cerr << "Failed to load submesh: " << get_submesh_path(modelPath, i) << std::endl;
            fclose(file);
            return false;
        }
        meshesVertices[i] = vertices;
        meshesIndices[i] = indices;
    }
    fclose(file);

    return true;
}