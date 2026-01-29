#pragma once

#include <cstdint>

struct ModelMesh
{
    uint32_t meshHash;
    uint32_t materialHash;
};

struct ModelMaterial
{
    uint32_t baseColorHash;
    uint32_t metallicHash;
    uint32_t roughnessHash;
    uint32_t normalHash;
    uint32_t aoHash;
};

struct ModelHeader
{
    uint32_t magic; // 'MODL0'
    uint32_t meshCount;
    uint32_t materialCount;
};
