#pragma once

#include <string>
#include <assets_format/texture_format.hpp>
#include <assets_format/mesh_format.hpp>
#include <assets_format/model_format.hpp>

class FormatValidator
{
public:
    static bool IsTextureHeaderValid(const TextureHeader &header)
    {
        return header.magic == 0x30584554; // 'TEX0'
    }

    static bool IsMeshHeaderValid(const MeshHeader &header)
    {
        return header.magic == 0x4853454D; // 'MESH0'
    }

    static bool IsModelHeaderValid(const ModelHeader &header)
    {
        return header.magic == 0x4C444F4D; // 'MODL0'
    }
};