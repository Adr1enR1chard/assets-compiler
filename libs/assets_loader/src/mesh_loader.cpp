#include <assets_loader/mesh_loader.hpp>
#include <assets_format/mesh_format.hpp>
#include <assets_format/format_validator.hpp>
#include <iostream>

bool MeshLoader::LoadMesh(const std::string &filePath, std::vector<VertexLayout> &vertices, std::vector<unsigned int> &indices)
{
    FILE *file = fopen(get_mesh_path(filePath).c_str(), "rb");
    if (!file)
    {
        std::cerr << "Failed to open mesh file: " << filePath << std::endl;
        return false;
    }

    MeshHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsMeshHeaderValid(header))
    {
        std::cerr << "Invalid mesh format: " << filePath << std::endl;
        fclose(file);
        return false;
    }

    vertices.resize(header.vertexCount);
    indices.resize(header.indexCount);

    fread(vertices.data(), sizeof(VertexLayout), header.vertexCount, file);
    fread(indices.data(), sizeof(unsigned int), header.indexCount, file);
    fclose(file);

    return true;
}