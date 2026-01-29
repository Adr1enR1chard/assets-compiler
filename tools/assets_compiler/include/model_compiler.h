#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assets_format/model_format.hpp>

void CompileModel(const char *inputPath, FILE *outputFile);

void CompileMesh(std::vector<VertexLayout> &vertices, std::vector<unsigned int> &indices, FILE *outputFile);

void CompileMaterial(MaterialDescriptor &outMaterial, FILE *outputFile);

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, FILE *outputFile, std::vector<std::vector<VertexLayout>> &meshesVertices, std::vector<std::vector<unsigned int>> &meshesIndices, std::vector<MaterialDescriptor> &materials);

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, std::vector<VertexLayout> &outVertices, std::vector<unsigned int> &outIndices);

void ProcessMaterial(const aiMaterial *material, MaterialDescriptor &outMaterial);