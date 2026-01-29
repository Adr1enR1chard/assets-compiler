#include <assets_loader/texture_loader.hpp>
#include <iostream>
#include <assets_format/format_validator.hpp>

bool TextureLoader::LoadTexture(const std::string &filePath, unsigned int &width, unsigned int &height, unsigned int &channels, std::vector<unsigned char> &data)
{
    FILE *file = fopen((filePath + ".asset").c_str(), "rb");
    if (!file)
    {
        std::cerr << "Failed to open texture file: " << filePath << std::endl;
        return false;
    }

    TextureHeader header;
    fread(&header, sizeof(header), 1, file);
    if (!FormatValidator::IsTextureHeaderValid(header))
    {
        std::cerr << "Invalid texture format: " << filePath << std::endl;
        fclose(file);
        return false;
    }

    width = header.width;
    height = header.height;
    channels = header.channels;

    unsigned int dataSize = width * height * channels;
    data.resize(dataSize);

    fread(data.data(), 1, dataSize, file);
    fclose(file);

    return true;
}