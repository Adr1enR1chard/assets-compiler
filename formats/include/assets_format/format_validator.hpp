#pragma once

#include <string>
#include <assets_format/texture_format.hpp>

class FormatValidator
{
public:
    static bool IsTextureHeaderValid(const TextureHeader &header)
    {
        return header.magic == 0x30584554; // 'TEX0'
    }
};