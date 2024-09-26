#include "csharp/dotnet_assembly.hpp"

#include "reflection/dotnet_reflection.hpp"
#include "rendering/light/point_light.hpp"
#include "scene/component/script_component.hpp"
#include "utils/logger.hpp"
#include "world/world.hpp"

using namespace XnorCore;

DotnetAssembly::DotnetAssembly(std::string filepath)
    : m_Filepath(std::move(filepath))
{
    m_Name = std::filesystem::path(m_Filepath).stem().string();
}

bool_t DotnetAssembly::Load(Coral::AssemblyLoadContext& alc)
{
    m_Assembly = &alc.LoadAssembly(m_Filepath);

    switch (m_Assembly->GetLoadStatus())
    {
        case Coral::AssemblyLoadStatus::Success:
            if (m_Assembly->GetName() == Dotnet::CoreProjectName)
                xnorCoreAssembly = this;
            return true;
        
        case Coral::AssemblyLoadStatus::FileNotFound:
            Logger::LogError("Couldn't find .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::FileLoadFailure:
            Logger::LogError("Couldn't load .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::InvalidFilePath:
            Logger::LogError("Invalid .NET assembly file path {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::InvalidAssembly:
            Logger::LogError("Invalid .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::UnknownError:
            Logger::LogError("An unknown error occured while loading .NET assembly file {}", m_Filepath);
            break;
    }

    return false;
}

void DotnetAssembly::ProcessTypes()
{
    if (!xnorCoreAssembly)
    {
        Logger::LogWarning("Couldn't process {} .NET assembly types because the XNOR assembly hasn't been loaded yet", m_Name);
        return;
    }

    const Coral::Type& scriptComponentType = xnorCoreAssembly->GetCoralAssembly()->GetType(Dotnet::XnorCoreNamespace + ".ScriptComponent");
    
    for (auto&& type : m_Assembly->GetTypes())
    {
        if (type->IsSubclassOf(scriptComponentType))
            DotnetReflection::RegisterScriptType(type->GetFullName());
        else if (type->IsEnum())
            DotnetReflection::RegisterEnumType(type->GetFullName(), m_Name);
    }
}

const Coral::ManagedAssembly* DotnetAssembly::GetCoralAssembly() const
{
    return m_Assembly;
}

const std::string& DotnetAssembly::GetFilename() const
{
    return m_Filepath;
}

const std::string& DotnetAssembly::GetName() const
{
    return m_Name;
}

void DotnetAssembly::ProcessScriptComponent(const Coral::Type& subclass)
{
    DotnetReflection::RegisterScriptType(subclass.GetFullName());
}
