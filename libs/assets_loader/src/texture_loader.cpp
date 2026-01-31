#include <assets_loader/texture_loader.hpp>
#include <iostream>
#include <assets_format/format_validator.hpp>

bool TextureLoader::LoadTexture(const std::string &filePath, LoadedTexture &texture)
{
    std::string texturePath = filePath + ".asset";
    FILE *file = fopen(texturePath.c_str(), "rb");
    if (!file)
    {
        std::cerr << "Failed to open texture file: " << texturePath << std::endl;
        return false;
    }

    TextureHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsTextureHeaderValid(header))
    {
        std::cerr << "Invalid texture format: " << texturePath << std::endl;
        fclose(file);
        return false;
    }

    texture.width = header.width;
    texture.height = header.height;
    texture.channels = header.channels;
    texture.format = header.format;

    // Read compressed or uncompressed data
    texture.data.resize(header.dataSize);
    fread(texture.data.data(), 1, header.dataSize, file);
    fclose(file);

    return true;
}

// Legacy interface for backwards compatibility (returns uncompressed data)
bool TextureLoader::LoadTexture(const std::string &filePath, unsigned int &width, unsigned int &height, unsigned int &channels, std::vector<unsigned char> &data)
{
    LoadedTexture texture;
    if (!LoadTexture(filePath, texture))
    {
        return false;
    }

    width = texture.width;
    height = texture.height;
    channels = texture.channels;

    // Note: For compressed textures, the data will be in compressed format
    // The caller should check the format and handle accordingly
    data = std::move(texture.data);

    return true;
}