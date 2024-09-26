#include "resource/model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "assimp/Exporter.hpp"
#include "assimp/Logger.hpp"
#include "rendering/rhi.hpp"
#include "utils/list.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Model::~Model()
{
    Rhi::DestroyModel(m_ModelId);
}



bool_t Model::Load(const aiMesh& loadedData)
{
    m_Vertices.resize(loadedData.mNumVertices);
    
    for (uint32_t i = 0; i < loadedData.mNumVertices; i++)
    {
        Vertex& vert = m_Vertices[i];
        vert.position = Vector3(&loadedData.mVertices[i].x);
        vert.normal = Vector3(&loadedData.mNormals[i].x);
        vert.textureCoord = Vector2(&loadedData.mTextureCoords[0][i].x);
    
        vert.tangent = Vector3(&loadedData.mBitangents[i].x);
        vert.bitangent = Vector3::Cross(vert.normal, vert.tangent);
    }

    for (uint32_t i = 0; i < loadedData.mNumBones; i++)
    {
        for (size_t j = 0; j < loadedData.mBones[i]->mNumWeights; j++)
        {
            const uint32_t vertexId = loadedData.mBones[i]->mWeights[j].mVertexId;
            const float_t weight = loadedData.mBones[i]->mWeights[j].mWeight;

            for (size_t k = 0; k < Vertex::MaxBoneWeight; k++)
            {
                if (Calc::Equals(m_Vertices[vertexId].boneIndices[k], -1))
                {
                    m_Vertices[vertexId].boneIndices[k] = static_cast<float_t>(i);
                    m_Vertices[vertexId].boneWeight[k] = weight;
                    break;
                }
            }
        }
    }

    if (loadedData.mNumBones != 0)
    {
        for (size_t i = 0; i < m_Vertices.size(); i++)
        {
            const float_t totalWeight = m_Vertices[i].boneWeight[0] + m_Vertices[i].boneWeight[1] + m_Vertices[i].boneWeight[2] + m_Vertices[i].boneWeight[3];

            if (!Calc::Equals(totalWeight, 1.f))
                Logger::LogError("Invalid bone weight : {} ; {}", i, totalWeight);
        }
    }

    m_Indices.resize(static_cast<size_t>(loadedData.mNumFaces) * 3);
    
    for (uint32_t i = 0; i < loadedData.mNumFaces; i++)
    {
        const aiFace& face = loadedData.mFaces[i];
        if (face.mNumIndices != 3)
        {
            Logger::LogError("Model data should be triangulated: {}", m_Name);
            return false;
        }
    
        const uint32_t baseIndex = i * 3;
        m_Indices[baseIndex + 0] = face.mIndices[0];
        m_Indices[baseIndex + 1] = face.mIndices[1];
        m_Indices[baseIndex + 2] = face.mIndices[2];
    }
    
    m_Loaded = true;

    ComputeAabb(loadedData.mAABB);

    return true;
}

void Model::CreateInInterface()
{
    m_ModelId = Rhi::CreateModel(m_Vertices, m_Indices);

    m_LoadedInInterface = true;
}

void Model::DestroyInInterface()
{
    Rhi::DestroyModel(m_ModelId);

    m_LoadedInInterface = false;
}

void Model::Unload()
{
    if (!m_Loaded)
        return;
    
    m_Vertices.clear();
    m_Indices.clear();

    m_Loaded = false;
}

bool_t Model::Save() const
{
    Assimp::Exporter exporter;
    aiScene scene;

    scene.mRootNode = new aiNode();

    scene.mMaterials = new aiMaterial*[1];
    scene.mMaterials[0] = nullptr;
    scene.mNumMaterials = 1;

    scene.mNumMeshes = 1;
    scene.mMeshes = new aiMesh*[1];
    scene.mMeshes[0] = new aiMesh;

    scene.mRootNode->mMeshes = new uint32_t[1];
    scene.mRootNode->mMeshes[0] = 0;
    scene.mRootNode->mNumMeshes = 1;

    aiMesh* const mesh = scene.mMeshes[0];

    scene.mMeshes[0]->mMaterialIndex = 0;

    List<aiVector3D> vertices(m_Vertices.size());
    List<aiVector3D> normals(m_Vertices.size());
    List<aiVector3D> texCoords(m_Vertices.size());
    List<aiVector3D> tangents(m_Vertices.size());
    
    for (size_t i = 0; i < m_Vertices.size(); i++)
    {
        vertices[i] = aiVector3D(m_Vertices[i].position.x, m_Vertices[i].position.y, m_Vertices[i].position.z);
        normals[i] = aiVector3D(m_Vertices[i].normal.x, m_Vertices[i].normal.y, m_Vertices[i].normal.z);
        texCoords[i] = aiVector3D(m_Vertices[i].textureCoord.x, m_Vertices[i].textureCoord.y, 0.f);
        tangents[i] = aiVector3D(m_Vertices[i].tangent.x, m_Vertices[i].tangent.y, m_Vertices[i].tangent.z);
    }

    mesh->mNumVertices = static_cast<uint32_t>(vertices.GetSize());

    mesh->mVertices = vertices.GetData();
    mesh->mNormals = normals.GetData();
    mesh->mTextureCoords[0] = texCoords.GetData();
    mesh->mTangents = tangents.GetData();

    List<aiFace> faces(m_Indices.size() / 3);

    uint32_t k = 0;
    for (uint32_t i = 0; i < static_cast<uint32_t>(m_Indices.size() / 3); i++)
    {
        faces[i].mIndices = new uint32_t[3];
        faces[i].mNumIndices = 3;

        faces[i].mIndices[0] = k;
        faces[i].mIndices[1] = k + 1;
        faces[i].mIndices[2] = k + 2;
        k = k + 3;
    }

    mesh->mFaces = faces.GetData();
    mesh->mNumFaces = static_cast<uint32_t>(m_Indices.size() / 3);

    const aiVector3D min = aiVector3D(aabb.GetMin().x, aabb.GetMin().y, aabb.GetMin().z);
    const aiVector3D max = aiVector3D(aabb.GetMax().x, aabb.GetMax().y, aabb.GetMax().z);
    mesh->mAABB = aiAABB(min, max);
    
    return exporter.Export(&scene, "obj", m_Name.c_str()) == aiReturn_SUCCESS;
}

uint32_t Model::GetId() const
{
    return m_ModelId;
}

const std::vector<Vertex>& Model::GetVertices() const
{
    return m_Vertices;
}

void Model::ComputeAabb(const aiAABB& assimpAabb)
{
    Vector3 min;
    Vector3 max;
    
    if (assimpAabb.mMax.x == 0.f && assimpAabb.mMax.y == 0.f && assimpAabb.mMax.z == 0.f &&
        assimpAabb.mMin.x == 0.f && assimpAabb.mMin.y == 0.f && assimpAabb.mMin.z == 0.f)
    {
        for (const Vertex& vertex : m_Vertices)
        {
            if (vertex.position.x < min.x)
                min.x = vertex.position.x;
            if (vertex.position.y < min.y)
                min.y = vertex.position.y;
            if (vertex.position.z < min.z)
                min.z = vertex.position.z;

            if (vertex.position.x > max.x)
                max.x = vertex.position.x;
            if (vertex.position.y > max.y)
                max.y = vertex.position.y;
            if (vertex.position.z > max.z)
                max.z = vertex.position.z;
        }
    }
    else
    {
        min = Vector3(&assimpAabb.mMin.x);
        max = Vector3(&assimpAabb.mMax.x);
    }

    aabb.SetMinMax(min, max);
}
