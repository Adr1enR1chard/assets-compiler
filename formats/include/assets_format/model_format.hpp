#pragma once

#include <cstdint>
#include <string>

struct VertexLayout
{
    float position[3];
    float normal[3];
    float texCoord[2];
    float tangent[3];
    float bitangent[3];
};

struct MaterialDescriptor
{
    uint32_t magic; // 'MATL0'
    char baseColorTexturePath[256] = {0};
    char metallicTexturePath[256] = {0};
    char roughnessTexturePath[256] = {0};
    char normalTexturePath[256] = {0};
    char aoTexturePath[256] = {0};
    bool useMetallicRoughnessTexture = false;
};

struct MeshHeader
{
    uint32_t magic; // 'MESH0'
    uint32_t vertexCount;
    uint32_t indexCount;
    uint32_t materialIndex;
};

struct ModelHeader
{
    uint32_t magic; // 'MODL0'
    uint32_t meshCount;
    uint32_t materialCount;
};
