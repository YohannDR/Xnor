#include "resource/resource_manager.hpp"

#include <array>
#include <execution>
#include <fstream>

#include "file/file_manager.hpp"
#include "resource/audio_track.hpp"
#include "resource/compute_shader.hpp"
#include "resource/font.hpp"
#include "resource/mesh.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/skeleton.hpp"
#include "resource/texture.hpp"

using namespace XnorCore;

void ResourceManager::LoadAll()
{
    Logger::LogInfo("Loading all resources from FileManager");

    auto&& start = std::chrono::system_clock::now();

    std::vector<Pointer<File>> files;
    FileManager::FindAll<File>([](Pointer<File> file) { return file->GetResource() == nullptr; }, &files);

    const size_t oldResourceCount = m_Resources.size();

    // Load resource data asynchronously
    std::for_each(
        std::execution::par,
        files.begin(),
        files.end(),
        [](const Pointer<File>& file) -> void
        {
            if (std::ranges::find(Texture::FileExtensions, file->GetExtension()) != Texture::FileExtensions.end())
                Load<Texture>(file, false);
            else if (std::ranges::find(Mesh::FileExtensions, file->GetExtension()) != Mesh::FileExtensions.end())
                Load<Mesh>(file, false);
            else if (std::ranges::find(Skeleton::FileExtensions, file->GetExtension()) != Skeleton::FileExtensions.end())
                Load<Skeleton>(file, false);
            else if (std::ranges::find(AudioTrack::FileExtensions, file->GetExtension()) != AudioTrack::FileExtensions.end())
                Load<AudioTrack>(file, false);
            else if (std::ranges::find(Font::FileExtensions, file->GetExtension()) != Font::FileExtensions.end())
                Load<Font>(file, false);
        }
    );

    // Do interface stuff synchronously (Rhi/Audio)
    for (Pointer<File>& file : files)
    {
        if (std::ranges::find(Shader::VertexFileExtensions, file->GetExtension()) != Shader::VertexFileExtensions.end() ||
            std::ranges::find(Shader::FragmentFileExtensions, file->GetExtension()) != Shader::FragmentFileExtensions.end() ||
            std::ranges::find(Shader::GeometryFileExtensions, file->GetExtension()) != Shader::GeometryFileExtensions.end())
        {
            Pointer<Shader> shader;

            // We use an underscore before the name to make sure it isn't used elsewhere
            const std::string&& filenameNoExtension = ReservedShaderPrefix + file->GetNameNoExtension();
            if (Contains(filenameNoExtension))
                shader = Get<Shader>(filenameNoExtension);
            else
                shader = Add<Shader>(filenameNoExtension);
        
            shader->Load(file);
        }
        else if (std::ranges::find(ComputeShader::ComputeFileExtensions, file->GetExtension()) != ComputeShader::ComputeFileExtensions.end())
        {
            Pointer<ComputeShader> computeShader;

            // We use an underscore before the name to make sure it isn't used elsewhere
            const std::string&& filenameNoExtension = ReservedShaderPrefix + file->GetNameNoExtension();
            if (Contains(filenameNoExtension))
                computeShader = Get<ComputeShader>(filenameNoExtension);
            else
                computeShader = Add<ComputeShader>(filenameNoExtension);
        
            computeShader->Load(file);
        }
        else
        {
            if (Contains(file))
                Get(file)->CreateInInterface();
        }
    }

    Logger::LogDebug(
        "Successfully loaded {} files in {} resources. Took {}",
        files.size(),
        m_Resources.size() - oldResourceCount,
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)
    );
}

void ResourceManager::LoadGuidMap()
{
    const Pointer<File> guidMap = FileManager::Get(GuidMapFilePath);
    const char_t* const dataRaw = guidMap.Get()->GetData();

    std::istringstream stream(dataRaw);
    std::string line;

    while (std::getline(stream, line))
    {
        const size_t guidPos = line.find_first_of(';');

        const std::string resourceName = line.substr(0, guidPos);
        const Guid guid = Guid::FromString(&line[guidPos + 1]);

        auto&& it = m_Resources.find(resourceName);

        if (it != m_Resources.end())
        {
            it->second->SetGuid(guid);
            m_GuidMap.emplace(guid, it->first);
        }
    }

    for (auto&& res : m_Resources)
    {
        auto it = std::ranges::find_if(m_GuidMap, [&res](auto&& p) -> bool_t { return p.second == res.first; });

        if (it == m_GuidMap.end())
        {
            m_GuidMap.emplace(res.second->GetGuid(), res.second->GetName());
        }
    }
}

void ResourceManager::SaveGuidMap()
{
    std::ofstream file(GuidMapFilePath);

    for (auto&& res : m_GuidMap)
    {
        file << res.second << ";" << static_cast<std::string>(res.first) << '\n';
    }

    file.close();
}

bool ResourceManager::Contains(const std::string& name)
{
    std::scoped_lock lock(m_ResourcesMutex);
    return m_Resources.contains(name);
}

bool ResourceManager::Contains(const Pointer<File>& file)
{
    std::scoped_lock lock(m_ResourcesMutex);
    return m_Resources.contains(file->GetPathString());
}

void ResourceManager::Rename(const std::string& name, const std::string& newName)
{
    Rename(Get(name), newName);
}

void ResourceManager::Rename(const Pointer<Resource>& resource, const std::string& newName)
{
    std::string&& oldName = resource->GetName();

    Logger::LogInfo("Renaming resource {} to {}", oldName, newName);

    // Create a new temporary strong reference of the resource to keep it alive until we insert it in the map again
    const Pointer newResource(resource, true);

    m_Resources.erase(oldName);
    // Here we also need to create a new strong reference as the last one will be deleted when going out of scope
    m_Resources[newName] = newResource.CreateStrongReference();
}

void ResourceManager::Unload(const std::string& name)
{
    Logger::LogDebug("Unloading resource {}", name);
    
    const auto&& resource = m_Resources.find(name);
    
    if (resource == m_Resources.end())
    {
        Logger::LogWarning("Attempt to unload an unknown resource: {}", name);
        return;
    }
    
    if (resource->second->IsLoadedInInterface())
        resource->second->DestroyInInterface();

    if (resource->second->IsLoaded())
        resource->second->Unload();

    m_Resources.erase(resource);
}

void ResourceManager::UnloadAll()
{
    Logger::LogInfo("Unloading all resources ({})", m_Resources.size());

    auto&& start = std::chrono::system_clock::now();
    
    for (auto& resource : m_Resources)
    {
        Logger::LogDebug("Unloading resource {}", resource.first);
        
        if (resource.second->IsLoadedInInterface())
            resource.second->DestroyInInterface();
        
        if (resource.second->IsLoaded())
            resource.second->Unload();
    }
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();

    SaveGuidMap();
    Logger::LogInfo("ResourceManager unload successful. Took {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start));
}
