#pragma once

#include <cstdint>
#include <string>

enum class TextureFormat : uint32_t
{
    UNCOMPRESSED_RGBA = 0,
    UNCOMPRESSED_RGB = 1,
    DXT1 = 2, // BC1 - RGB or RGBA with 1-bit alpha (8 bytes per 4x4 block)
    DXT5 = 3  // BC3 - RGBA with full alpha (16 bytes per 4x4 block)
};

struct TextureHeader
{
    uint32_t magic; // 'TEX0'
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t mipLevels;   // 1 for now
    TextureFormat format; // Compression format
    uint32_t dataSize;    // Size of compressed/uncompressed data in bytes
};