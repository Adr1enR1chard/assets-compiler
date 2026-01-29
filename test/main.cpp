#include <iostream>
#include <filesystem>
#include <assets_loader/model_loader.hpp>
#include <assets_loader/texture_loader.hpp>

int main()
{
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

    std::cout << "Running asset loader tests..." << std::endl;
    std::cout << "--------------------------------" << std::endl;

    std::cout
        << "Testing texture loading..." << std::endl;
    {
        unsigned int width, height, channels;
        std::vector<unsigned char> data;
        bool result = TextureLoader::LoadTexture("assets/textures/black1x1.png", width, height, channels, data);
        if (!result)
        {
            std::cerr << "Failed to load texture." << std::endl;
            return 1;
        }
        if (!(width == 1 && height == 1 && channels == 3))
        {
            std::cerr << "Texture properties mismatch." << std::endl;
            return 1;
        }
        if (data.empty())
        {
            std::cerr << "Texture data is empty." << std::endl;
            return 1;
        }
        std::cout << "Texture loaded successfully: " << width << "x" << height << " with " << channels << " channels." << std::endl;
        if (!(data[0] == 0 && data[1] == 0 && data[2] == 0))
        {
            std::cerr << "Texture pixel data mismatch." << std::endl;
            return 1;
        }
    }

    std::cout << "Testing model loading..." << std::endl;
    {
        std::vector<std::vector<VertexLayout>> meshesVertices;
        std::vector<std::vector<unsigned int>> meshesIndices;
        std::vector<MaterialDescriptor> materials;
        bool result = ModelLoader::LoadModel("assets/models/sword/scene.gltf", meshesVertices, meshesIndices, materials);
        if (!result)
        {
            std::cerr << "Failed to load model." << std::endl;
            return 1;
        }
        if (meshesVertices.size() != 2 || meshesIndices.size() != 2)
        {
            std::cerr << "Model meshes count mismatch. Expected 2, got " << meshesVertices.size() << " and " << meshesIndices.size() << std::endl;
            return 1;
        }
        if (meshesVertices[0].size() != 2369 || meshesIndices[0].size() != 9414)
        {
            std::cerr << "First mesh data is empty." << std::endl;
            return 1;
        }
        if (materials.size() != 2)
        {
            std::cerr << "Model materials count mismatch. Expected 2, got " << materials.size() << std::endl;
            return 1;
        }
        if (std::string(materials[0].baseColorTexturePath) != "textures/Material-Ancient-Sword_baseColor.png")
        {
            std::cerr << "First material base color texture path mismatch. Found " << materials[0].baseColorTexturePath << std::endl;
            return 1;
        }
    }
}