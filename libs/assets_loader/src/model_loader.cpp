#include <assets_loader/model_loader.hpp>
#include <assets_format/model_format.hpp>
#include <assets_format/format_validator.hpp>

#include <iostream>

bool ModelLoader::LoadModel(const std::string &filePath, std::vector<std::vector<VertexLayout>> &meshesVertices, std::vector<std::vector<unsigned int>> &meshesIndices, std::vector<MaterialDescriptor> &materials)
{
    std::string modelPath = filePath + ".asset";
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
        std::cerr << "Expected magic: 0x4C444F4D, Found magic: 0x" << std::hex << header.magic << std::dec << std::endl;
        fclose(file);
        return false;
    }

    meshesVertices.resize(header.meshCount);
    meshesIndices.resize(header.meshCount);

    for (uint32_t i = 0; i < header.meshCount; ++i)
    {
        std::vector<VertexLayout> vertices;
        std::vector<unsigned int> indices;
        if (!LoadSubmesh(file, vertices, indices))
        {
            fclose(file);
            return false;
        }
        meshesVertices[i] = vertices;
        meshesIndices[i] = indices;
    }

    materials.resize(header.materialCount);
    for (uint32_t i = 0; i < header.materialCount; ++i)
    {
        if (!LoadMaterial(file, materials[i]))
        {
            fclose(file);
            return false;
        }
    }

    fclose(file);

    return true;
}

bool ModelLoader::LoadSubmesh(FILE *file, std::vector<VertexLayout> &vertices, std::vector<unsigned int> &indices)
{
    MeshHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsMeshHeaderValid(header))
    {
        std::cerr << "Invalid mesh format. Found magic: 0x" << std::hex << header.magic << std::dec << std::endl;
        return false;
    }

    vertices.resize(header.vertexCount);
    indices.resize(header.indexCount);

    fread(vertices.data(), sizeof(VertexLayout), header.vertexCount, file);
    fread(indices.data(), sizeof(unsigned int), header.indexCount, file);

    return true;
}

bool ModelLoader::LoadMaterial(FILE *file, MaterialDescriptor &material)
{
    fread(&material, sizeof(MaterialDescriptor), 1, file);
    if (!FormatValidator::IsMaterialDescriptorValid(material))
    {
        std::cerr << "Invalid material format. Found magic: 0x" << std::hex << material.magic << std::dec << std::endl;
        return false;
    }
    return true;
}