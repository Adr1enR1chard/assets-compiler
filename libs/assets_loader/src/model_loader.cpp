#include <assets_loader/model_loader.hpp>
#include <assets_format/model_format.hpp>
#include <assets_format/format_validator.hpp>

#include <iostream>

bool ModelLoader::LoadModel(const std::string &filePath, std::vector<MeshData> &meshes, std::vector<MaterialDescriptor> &materials)
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

    meshes.resize(header.meshCount);
    for (uint32_t i = 0; i < header.meshCount; ++i)
    {
        MeshData mesh;
        if (!LoadSubmesh(file, mesh))
        {
            fclose(file);
            return false;
        }
        meshes[i] = mesh;
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

bool ModelLoader::LoadSubmesh(FILE *file, MeshData &mesh)
{
    MeshHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsMeshHeaderValid(header))
    {
        std::cerr << "Invalid mesh format. Found magic: 0x" << std::hex << header.magic << std::dec << std::endl;
        return false;
    }

    mesh.vertices.resize(header.vertexCount);
    mesh.indices.resize(header.indexCount);

    fread(mesh.vertices.data(), sizeof(VertexLayout), header.vertexCount, file);
    fread(mesh.indices.data(), sizeof(unsigned int), header.indexCount, file);
    mesh.localTransform = header.localTransform;

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