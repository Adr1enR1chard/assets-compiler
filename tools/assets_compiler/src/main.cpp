/// @file main.cpp
/// @brief Entry point for the asset compiler tool. The tool processes and converts various asset files into optimized formats for use in the game engine.
/// @author Adrien RICHARD

#include <iostream>
#include <filesystem>
#include "utils/format.h"
#include "texture_compiler.h"
#include "model_compiler.h"

int main(int argc, char **argv)
{
    // Retrieve assets source and destination directories from command line arguments
    // Example: ./asset_compiler /path/to/source /path/to/destination
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <source_directory> <destination_directory>" << std::endl;
        return 1;
    }

    const char *sourceDir = argv[1];
    const char *destDir = argv[2];

    std::cout << "Asset Compiler Tool" << std::endl;
    std::cout << "Source Directory: " << sourceDir << std::endl;
    std::cout << "Destination Directory: " << destDir << std::endl;
    std::cout << "Starting asset processing..." << std::endl;

    // if relative path, make absolute
    std::filesystem::path sourcePath = std::filesystem::absolute(sourceDir);
    std::filesystem::path destPath = std::filesystem::absolute(destDir);

    if (!std::filesystem::exists(sourcePath) || !std::filesystem::is_directory(sourcePath))
    {
        std::cerr << "Source directory does not exist or is not a directory: " << sourcePath << std::endl;
        return 1;
    }

    // Iterate through the source directory, process each asset file, and save the optimized version to the destination directory
    for (const auto &entry : std::filesystem::recursive_directory_iterator(sourcePath))
    {
        if (entry.is_regular_file() && entry.path().has_extension())
        {
            const auto &filePath = entry.path();

            AssetType assetType = GetAssetType(filePath);
            if (assetType == AssetType::Unknown)
                continue;

            std::filesystem::path relativePath = std::filesystem::relative(filePath, sourcePath);
            std::filesystem::path outputPath = destPath / relativePath;
            std::filesystem::create_directories(outputPath.parent_path());

            FILE *outputFile = fopen((outputPath.string() + ".asset").c_str(), "wb");
            if (!outputFile)
            {
                std::cerr << "Failed to open output file: " << outputPath << std::endl;
                continue;
            }

            switch (assetType)
            {
            case AssetType::Texture:
                // Ensure the output directory exists

                // Compile the texture
                CompileTexture(filePath.string().c_str(), outputFile);
                std::cout << "Compiled texture: " << filePath << " -> " << outputPath << std::endl;
                break;

            case AssetType::Model:
                CompileModel(filePath.string().c_str(), outputFile);
                std::cout << "Compiled model: " << filePath << " -> " << outputPath << std::endl;
                break;

            default:
                std::cout << "Unsupported asset type for file: " << filePath << std::endl;
                break;
            }

            fclose(outputFile);
        }
    }

    return 0;
}