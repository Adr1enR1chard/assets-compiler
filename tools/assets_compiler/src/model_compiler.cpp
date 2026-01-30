#include "model_compiler.h"
#include "assets_format/model_format.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>

glm::mat4 AiMatrixToGlmMat4(const aiMatrix4x4 &from)
{
    glm::mat4 to;
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

void CompileModel(const char *inputPath, FILE *outputFile)
{
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(inputPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp error: " << import.GetErrorString() << std::endl;
        return;
    }

    int meshCounter = 0;
    std::vector<MeshData> meshes;
    std::vector<MaterialDescriptor> materials;
    ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f), outputFile, meshes, materials);

    ModelHeader header{
        0x4C444F4D, // 'MODL0'
        static_cast<uint32_t>(meshes.size()),
        static_cast<uint32_t>(materials.size())};
    fwrite(&header, sizeof(header), 1, outputFile);

    for (size_t i = 0; i < meshes.size(); i++)
    {
        CompileMesh(meshes[i], outputFile);
    }

    for (size_t i = 0; i < materials.size(); i++)
    {
        CompileMaterial(materials[i], outputFile);
    }
}

void CompileMesh(MeshData &meshData, FILE *outputFile)
{
    MeshHeader header{
        0x4853454D, // 'MESH0'
        static_cast<uint32_t>(meshData.vertices.size()),
        static_cast<uint32_t>(meshData.indices.size()),
        0, // materialIndex to be set later,
        meshData.localTransform};
    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(meshData.vertices.data(), sizeof(VertexLayout), meshData.vertices.size(), outputFile);
    fwrite(meshData.indices.data(), sizeof(unsigned int), meshData.indices.size(), outputFile);
}

void CompileMaterial(MaterialDescriptor &outMaterial, FILE *outputFile)
{
    outMaterial.magic = 0x4C54414D; // 'MATL0'
    fwrite(&outMaterial, sizeof(MaterialDescriptor), 1, outputFile);
}

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, FILE *outputFile, std::vector<MeshData> &meshes, std::vector<MaterialDescriptor> &materials)
{
    glm::mat4 nodeTransform = parentTransform * AiMatrixToGlmMat4(node->mTransformation);

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        ProcessMesh(mesh, scene, nodeTransform, meshes.emplace_back());
        ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex], materials.emplace_back());
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, nodeTransform, outputFile, meshes, materials);
    }
}

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, MeshData &outMesh)
{
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexLayout vertex;
        vertex.position[0] = mesh->mVertices[i].x;
        vertex.position[1] = mesh->mVertices[i].y;
        vertex.position[2] = mesh->mVertices[i].z;
        vertex.normal[0] = mesh->mNormals[i].x;
        vertex.normal[1] = mesh->mNormals[i].y;
        vertex.normal[2] = mesh->mNormals[i].z;
        if (mesh->mTangents)
        {
            vertex.tangent[0] = mesh->mTangents[i].x;
            vertex.tangent[1] = mesh->mTangents[i].y;
            vertex.tangent[2] = mesh->mTangents[i].z;
        }
        if (mesh->mBitangents)
        {
            vertex.bitangent[0] = mesh->mBitangents[i].x;
            vertex.bitangent[1] = mesh->mBitangents[i].y;
            vertex.bitangent[2] = mesh->mBitangents[i].z;
        }
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord[0] = mesh->mTextureCoords[0][i].x;
            vertex.texCoord[1] = mesh->mTextureCoords[0][i].y;
        }
        outMesh.vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            outMesh.indices.push_back(face.mIndices[j]);
    }
    outMesh.localTransform = transform;
}

void ProcessMaterial(const aiMaterial *material, MaterialDescriptor &outMaterial)
{
    aiString texturePath;
    if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &texturePath) == AI_SUCCESS)
    {
        std::strncpy(outMaterial.baseColorTexturePath, texturePath.C_Str(), sizeof(outMaterial.baseColorTexturePath));
    }
    else if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
    {
        std::strncpy(outMaterial.baseColorTexturePath, texturePath.C_Str(), sizeof(outMaterial.baseColorTexturePath));
    }

    if (material->GetTexture(aiTextureType_GLTF_METALLIC_ROUGHNESS, 0, &texturePath) == AI_SUCCESS)
    {
        std::strncpy(outMaterial.metallicTexturePath, texturePath.C_Str(), sizeof(outMaterial.metallicTexturePath));
        outMaterial.useMetallicRoughnessTexture = true;
    }
    else
    {
        if (material->GetTexture(aiTextureType_METALNESS, 0, &texturePath) == AI_SUCCESS)
        {
            std::strncpy(outMaterial.metallicTexturePath, texturePath.C_Str(), sizeof(outMaterial.metallicTexturePath));
        }

        if (material->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texturePath) == AI_SUCCESS)
        {
            std::strncpy(outMaterial.roughnessTexturePath, texturePath.C_Str(), sizeof(outMaterial.roughnessTexturePath));
        }
    }

    if (material->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == AI_SUCCESS)
    {
        std::strncpy(outMaterial.normalTexturePath, texturePath.C_Str(), sizeof(outMaterial.normalTexturePath));
    }

    if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath) == AI_SUCCESS)
    {
        std::strncpy(outMaterial.aoTexturePath, texturePath.C_Str(), sizeof(outMaterial.aoTexturePath));
    }
}
