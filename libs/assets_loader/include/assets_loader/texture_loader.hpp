#pragma once

#include <string>
#include <vector>

#include <assets_format/texture_format.hpp>

class TextureLoader
{
public:
    static bool LoadTexture(const std::string &filePath, int &width, int &height, int &channels, std::vector<unsigned char> &data);
};