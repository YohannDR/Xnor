#include "resource/mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Mesh::~Mesh()
{
    models.Clear();
}

bool_t Mesh::Load(const uint8_t* buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData);
    const std::string folderPath = m_File->GetPathNoExtension() + '\\';

    if (!scene)
        return false;

    Pointer<Skeleton> skeletonRef = nullptr;

    const bool_t hasSkeleton = LoadMesh(*scene, &skeletonRef);

    if (hasSkeleton)
    {
        for (uint32_t i = 0; i < scene->mNumAnimations; i++)
        {
            Pointer<Animation> animation = ResourceManager::Add<Animation>(folderPath + std::string(scene->mAnimations[i]->mName.C_Str()) + ".anim");
        
            animation->Load(*scene->mAnimations[i]);
            animation->BindSkeleton(m_Skeletons[0]);
            m_Animations.Add(animation);
        }
    }

    LoadTexture(*scene);
    /*
    for (uint32_t i = 0; i < scene->mNumTextures; i++)
    {
        std::string fileName = GetTextureFileName(folderPath,scene->mTextures[i]->mFilename.C_Str(),scene->mTextures[i]->achFormatHint, i);
    
        const std::string fullName = folderPath + fileName;

        if (FileManager::Contains(fullName))
        {
            
        }
        
        const size_t pos = fileName.find_first_of('\\');
        if (pos != std::string::npos)
        {
            const std::string subFolder = folderPath + fileName.substr(0, pos);
            //FileManager::AddDirectory(subFolder);
        }
        Pointer<Texture> texture = ResourceManager::Add<Texture>(fullName);

        int64_t size = 0;
        if (scene->mTextures[i]->mHeight == 0)
            size = scene->mTextures[i]->mWidth;
        else
            size = static_cast<int64_t>(static_cast<uint64_t>(scene->mTextures[i]->mWidth) * static_cast<uint64_t>(scene->mTextures[i]->mHeight) * sizeof(aiTexel));

        texture->Load(reinterpret_cast<const uint8_t*>(scene->mTextures[i]->pcData), size);
        texture->SetIsEmbedded();
        texture->Save();
    }*/

    /*
    if (textures.GetSize() >= 1)
    {
        aiString textureName;
        scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
        
        material.albedoTexture = Pointer<Texture>::New(*textures[0]);
    }*/
    ComputeAabb();
    
    return true;
}

void Mesh::CreateInInterface()
{
    for (size_t i = 0; i < models.GetSize(); i++)
        models[i]->CreateInInterface();
    
    m_LoadedInInterface = true;
}

Pointer<Animation> Mesh::GetAnimation(const size_t id)
{
    if (id >= m_Animations.GetSize())
        return nullptr;
    
    return m_Animations[id];
}

std::string Mesh::GetTextureFileName(const std::string& textureName, const std::string& textureFormat)
{
    std::string returnName;
    const std::string baseNameCopy = textureName;
    
    const size_t last =  baseNameCopy.find_last_of('\\') + 1;
        
    for (size_t i = last; i < baseNameCopy.size();i++)
        returnName.push_back(baseNameCopy[i]);

    const std::filesystem::path path(returnName);
    returnName = path.stem().generic_string();
    returnName += ( "." + textureFormat);
    
    return returnName;
}

bool_t Mesh::LoadMesh(const aiScene& scene,Pointer<Skeleton>* outSkeleton)
{
    const std::string folderPath = m_File->GetPathNoExtension() + '\\';
    //FileManager::AddDirectory(folderPath);

    uint32_t nbrOfSkeleton = 0;
    bool_t hasSkeleton = false;
    Pointer<Skeleton> skeleton = nullptr;
    
    for (uint32_t i = 0; i < scene.mNumMeshes; i++)
    {
        const std::string meshName = scene.mMeshes[i]->mName.C_Str();
        const std::string fullName = folderPath + std::string(meshName) + std::string(".obj");
        Pointer<Model> model = nullptr;
        
        if (!ResourceManager::Contains(fullName))
        {
            model = ResourceManager::Add<Model>(fullName);

            if (!model->Load(*scene.mMeshes[i]))
            {
                ResourceManager::Unload(model);
                return false;
            }
            
            models.Add(model);

            if (nbrOfSkeleton > 0)
            {
                Logger::LogWarning("Xnor doesn't handle multiple skeletons from one file {}", m_File->GetPath());
                continue;
            }
        }
        else
        {
           model = ResourceManager::Get<Model>(fullName);
        }

        if (scene.mMeshes[i]->HasBones())
        {
            std::string resourceName = folderPath + std::string(scene.mAnimations[i]->mName.C_Str()) + ".skel";
            if (ResourceManager::Contains(resourceName))
            {
                skeleton = ResourceManager::Get<Skeleton>(resourceName);
                *outSkeleton = skeleton;
                return true;
            }
            else
            {
                skeleton = ResourceManager::Add<Skeleton>(resourceName);
            }
            
            skeleton->Load(*scene.mMeshes[i], *scene.mRootNode);

            if (i < scene.mNumAnimations)
                skeleton->Load(scene, *scene.mAnimations[i]);

            skeleton->ReorderBones();
            skeleton->mesh = this;

            m_Skeletons.Add(skeleton);
            nbrOfSkeleton++;
            hasSkeleton = true;
        }
        
    }

    if (hasSkeleton)
        *outSkeleton = skeleton;
    
    return hasSkeleton;
}

void Mesh::LoadTexture(const aiScene& scene)
{
    for (uint32_t i = 0; i < scene.mNumTextures; i++)
    {
        std::string textureAssimpname = scene.mTextures[i]->mFilename.C_Str();

        if (textureAssimpname.empty())
            textureAssimpname = m_File->GetNameNoExtension();
        
        std::string fileName = GetTextureFileName(textureAssimpname,scene.mTextures[i]->achFormatHint);

        std::filesystem::path p(m_File->GetPath());
        std::string parentPath = p.parent_path().generic_string();
        
        std::string filePath = parentPath + "/" + fileName;
        
        if (FileManager::Contains(filePath))
        {
             continue;
        }
        
        Pointer<Texture> texture = ResourceManager::Add<Texture>(fileName);
        texture->SetName(fileName);
        texture->SetFile(FileManager::Add( std::filesystem::path(filePath)));

        int64_t size = 0;
        if (scene.mTextures[i]->mHeight == 0)
            size = scene.mTextures[i]->mWidth;
        else
            size = static_cast<int64_t>(static_cast<uint64_t>(scene.mTextures[i]->mWidth) * static_cast<uint64_t>(scene.mTextures[i]->mHeight) * sizeof(aiTexel));

        texture->Load(reinterpret_cast<const uint8_t*>(scene.mTextures[i]->pcData), size);
        texture->SetIsEmbedded();
        texture->Save();
    }
}

void Mesh::ComputeAabb()
{
    Vector3 aabbMin = Vector3(std::numeric_limits<float_t>::max());
    Vector3 aabbMax =  Vector3(std::numeric_limits<float_t>::min());
    
    for (size_t i = 0; i < models.GetSize(); ++i)
    {
        if (!models[i].IsValid())
            continue;

        const Bound& bound = models[i]->aabb;
        const Vector3& min = bound.GetMin();
        const Vector3& max = bound.GetMax();

        if (min.x < aabbMin.x)
            aabbMin.x = min.x;
        if (min.y < aabbMin.y)
            aabbMin.y = min.y;
        if (min.z < aabbMin.z)
            aabbMin.z = min.z;

        if (max.x > aabbMax.x)
            aabbMax.x = max.x;
        if (max.y > aabbMax.y)
            aabbMax.y = max.y;
        if (max.z > aabbMax.z)
            aabbMax.z = max.z;
    }

    aabb.SetMinMax(aabbMin, aabbMax);
}

