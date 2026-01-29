#pragma once

#include <glm/glm.hpp>
#include <assimp/scene.h>

void CompileModel(const char *inputPath, const char *outputPath);

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, const char *outputPath, int &meshCounter);

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, const char *outputPath, int meshIndex);

void ProcessMaterial(const aiMaterial *material, const char *outputPath);
void ProcessTexture(const aiString &texturePath, const char *outputPath);