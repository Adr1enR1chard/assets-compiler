#pragma once

#include <string>
#include <vector>

#include <assets_format/texture_format.hpp>

struct LoadedTexture
{
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    TextureFormat format;
    std::vector<unsigned char> data;
};

class TextureLoader
{
public:
    static bool LoadTexture(const std::string &filePath, LoadedTexture &texture);

    // Legacy interface for backwards compatibility
    static bool LoadTexture(const std::string &filePath, unsigned int &width, unsigned int &height, unsigned int &channels, std::vector<unsigned char> &data);
};