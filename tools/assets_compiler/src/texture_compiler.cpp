#include "texture_compiler.h"
#include <assets_format/texture_format.hpp>

#include <iostream>
#include <filesystem>
#include <stb_image/stb_image.h>

void CompileTexture(const char *inputPath, FILE *file)
{
    int width, height, channels;
    unsigned char *data = stbi_load(inputPath, &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << inputPath << std::endl;
        return;
    }

    // Simple example: just write width, height, channels as binary data
    if (!file)
    {
        std::cerr << "Invalid output file pointer." << std::endl;
        stbi_image_free(data);
        return;
    }

    TextureHeader header{
        0x30584554, // 'TEX0'
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        static_cast<uint32_t>(channels),
        1 // mipLevels
    };

    fwrite(&header, sizeof(header), 1, file);
    fwrite(data, sizeof(unsigned char), width * height * channels, file);

    stbi_image_free(data);
}