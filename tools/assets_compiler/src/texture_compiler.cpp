#include "texture_compiler.h"
#include <assets_format/texture_format.hpp>

#include <iostream>
#include <filesystem>
#include <vector>
#include <stb_image/stb_image.h>
#include <stb_dxt/stb_dxt.h>

// Helper function to compress a 4x4 block
static void CompressBlock(const unsigned char *rgba, unsigned char *dest, bool useAlpha)
{
    if (useAlpha)
    {
        // DXT5: 16 bytes per block (8 bytes alpha + 8 bytes color)
        stb_compress_dxt_block(dest, rgba, 1, STB_DXT_HIGHQUAL);
    }
    else
    {
        // DXT1: 8 bytes per block
        stb_compress_dxt_block(dest, rgba, 0, STB_DXT_HIGHQUAL);
    }
}

void CompileTexture(const char *inputPath, FILE *file)
{
    int width, height, channels;
    unsigned char *data = stbi_load(inputPath, &width, &height, &channels, 4); // Force RGBA
    if (!data)
    {
        std::cerr << "Failed to load texture: " << inputPath << std::endl;
        return;
    }

    if (!file)
    {
        std::cerr << "Invalid output file pointer." << std::endl;
        stbi_image_free(data);
        return;
    }

    // Determine compression format based on alpha channel
    bool hasAlpha = (channels == 4);
    TextureFormat format = hasAlpha ? TextureFormat::DXT5 : TextureFormat::DXT1;

    // Calculate number of blocks (round up to multiple of 4)
    int blocksX = (width + 3) / 4;
    int blocksY = (height + 3) / 4;
    int blockSize = hasAlpha ? 16 : 8; // DXT5 = 16 bytes, DXT1 = 8 bytes
    uint32_t compressedSize = blocksX * blocksY * blockSize;

    // Allocate buffer for compressed data
    std::vector<unsigned char> compressedData(compressedSize);

    // Compress texture in 4x4 blocks
    for (int by = 0; by < blocksY; ++by)
    {
        for (int bx = 0; bx < blocksX; ++bx)
        {
            // Extract 4x4 block (with padding if needed)
            unsigned char block[64]; // 4x4 pixels * 4 channels (RGBA)

            for (int y = 0; y < 4; ++y)
            {
                for (int x = 0; x < 4; ++x)
                {
                    int px = bx * 4 + x;
                    int py = by * 4 + y;

                    // Clamp to texture boundaries
                    if (px >= width)
                        px = width - 1;
                    if (py >= height)
                        py = height - 1;

                    int srcIdx = (py * width + px) * 4;
                    int dstIdx = (y * 4 + x) * 4;

                    block[dstIdx + 0] = data[srcIdx + 0]; // R
                    block[dstIdx + 1] = data[srcIdx + 1]; // G
                    block[dstIdx + 2] = data[srcIdx + 2]; // B
                    block[dstIdx + 3] = data[srcIdx + 3]; // A
                }
            }

            // Compress the block
            int blockIndex = (by * blocksX + bx) * blockSize;
            CompressBlock(block, &compressedData[blockIndex], hasAlpha);
        }
    }

    // Write header
    TextureHeader header{
        0x30584554, // 'TEX0'
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        static_cast<uint32_t>(channels),
        1, // mipLevels
        format,
        compressedSize};

    fwrite(&header, sizeof(header), 1, file);
    fwrite(compressedData.data(), 1, compressedSize, file);

    std::cout << "Compressed texture: " << inputPath << " (" << width << "x" << height
              << ") from " << (width * height * 4) << " to " << compressedSize
              << " bytes (" << (100.0f * compressedSize / (width * height * 4)) << "%)" << std::endl;

    stbi_image_free(data);
}