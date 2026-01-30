#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assets_format/model_format.hpp>

void CompileModel(const char *inputPath, FILE *outputFile);

void CompileMesh(MeshData &meshData, FILE *outputFile);

void CompileMaterial(MaterialDescriptor &outMaterial, FILE *outputFile);

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, FILE *outputFile, std::vector<MeshData> &meshes, std::vector<MaterialDescriptor> &materials);

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, MeshData &outMesh);

void ProcessMaterial(const aiMaterial *material, MaterialDescriptor &outMaterial);