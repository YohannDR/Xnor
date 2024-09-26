#pragma once

#include <string>

#include <Coral/Assembly.hpp>

#include "core.hpp"

/// @file dotnet_assembly.hpp
/// @brief Defines the DotnetAssembly class.

BEGIN_XNOR_CORE

/// @brief The DotnetAssembly class is used to manage a .NET assembly that was loaded using DotnetRuntime::LoadAssembly.
class DotnetAssembly
{
public:
    /// @brief The XNOR Engine Core .NET assembly. This is automatically loaded along with the .NET runtime.
    XNOR_ENGINE static inline const DotnetAssembly* xnorCoreAssembly = nullptr;

    /// @brief Constructs a .NET assembly from the given @p filepath.
    XNOR_ENGINE explicit DotnetAssembly(std::string filepath);

    /// @brief Loads the DotnetAssembly using the given AssemblyLoadContext.
    XNOR_ENGINE bool_t Load(Coral::AssemblyLoadContext& alc);

    /// @brief Processes the currently-loaded assembly.
    XNOR_ENGINE void ProcessTypes();

    /// @brief Returns the wrapped Coral ManagedAssembly.
    [[nodiscard]]
    XNOR_ENGINE const Coral::ManagedAssembly* GetCoralAssembly() const;

    /// @brief Returns the file name of this assembly. This is effectively equivalent to @c GetName() @c + @c ".dll".
    [[nodiscard]]
    XNOR_ENGINE const std::string& GetFilename() const;

    /// @brief Returns the name of this assembly.
    [[nodiscard]]
    XNOR_ENGINE const std::string& GetName() const;

private:
    Coral::ManagedAssembly* m_Assembly = nullptr;

    std::string m_Filepath;

    std::string m_Name;

    static void ProcessScriptComponent(const Coral::Type& subclass);
};

END_XNOR_CORE
