#pragma once

#include <Coral/HostInstance.hpp>

#include "core.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "utils/logger.hpp"

/// @file dotnet_runtime.hpp
/// @brief Defines the DotnetRuntime class.

BEGIN_XNOR_CORE

/// @brief Static class used to manage the current .NET runtime.
class DotnetRuntime
{
    STATIC_CLASS(DotnetRuntime)
    
public:
    enum class BuildResult
    {
        Unknown,
        Success,
        Warning,
        Error
    };
    
    /// @brief Initializes the .NET runtime.
    XNOR_ENGINE static bool_t Initialize();

    /// @brief Shutdowns the current .NET runtime.
    XNOR_ENGINE static void Shutdown();

    /// @brief Loads the assembly with the given @p name.
    ///
    /// This assembly should be in the 'DotnetAssemblies' directory, within the working directory.
    ///
    /// @param name The name of the assembly to load. This should be the file name without the '.dll' extension.
    XNOR_ENGINE static bool_t LoadAssembly(const std::string& name);

    /// @brief Returns the loaded assembly with the given @p name.
    XNOR_ENGINE static DotnetAssembly* GetAssembly(const std::string& name);

    /// @brief Tells the .NET Garbage Collector to collect unused memory now. If @p wait is set, the function only returns when the GC has finished its task.
    XNOR_ENGINE static void GcCollect(bool_t wait = true);

    /// @brief Returns the loaded Game assembly.
    XNOR_ENGINE static DotnetAssembly* GetGameAssembly();

    /// @brief Unloads all loaded assemblies.
    ///
    /// @param reloadContext Unloading all assemblies means unloading the AssemblyLoadContext. This parameter describes whether to reload the context afterward.
    XNOR_ENGINE static void UnloadAllAssemblies(bool_t reloadContext = false);

    /// @brief Reloads all loaded assemblies.
    XNOR_ENGINE static void ReloadAllAssemblies();

    /// @brief Builds the Game .NET project.
    XNOR_ENGINE static bool_t BuildGameProject(bool_t asynchronous = true);

    /// @brief Builds the Game .NET project and reloads the resulting assembly. This can be quite long, consider calling this function on a worker thread.
    /// @param recreateScriptInstances Whether to recreate the .NET ScriptComponent instances after the reload.
    XNOR_ENGINE static void BuildAndReloadProject(bool_t recreateScriptInstances = true);

    /// @brief Returns whether the runtime is initialized.
    [[nodiscard]]
    XNOR_ENGINE static bool_t GetInitialized();

    /// @brief Returns whether a thread is currently building/reloading the .NET project.
    [[nodiscard]]
    XNOR_ENGINE static bool_t IsReloadingProject();

    /// @brief Returns the current building/reloading progress of the .NET project. Only valid if @c IsReloadingProject() returns @c true
    [[nodiscard]]
    XNOR_ENGINE static float_t GetProjectReloadingProgress();

    /// @brief Returns the last project build result.
    [[nodiscard]]
    XNOR_ENGINE static BuildResult GetProjectLastBuildResult();

private:
    XNOR_ENGINE static constexpr int32_t DotnetVersion = 5;
    
    static Coral::HostSettings m_Settings;
    
    XNOR_ENGINE static inline Coral::HostInstance m_Runtime;
    
    XNOR_ENGINE static inline Coral::AssemblyLoadContext m_Alc;

    XNOR_ENGINE static inline bool_t m_Initialized = false;

    XNOR_ENGINE static inline std::vector<DotnetAssembly*> m_LoadedAssemblies;
    
    XNOR_ENGINE static inline std::filesystem::path m_AssembliesPath;

    XNOR_ENGINE static inline bool_t m_ReloadingProject = false;

    /// @brief Project reloading progress between 0 and 1. Only valid if @c m_ReloadingProject is @c true
    XNOR_ENGINE static inline float_t m_ProjectReloadingProgress = 0.f;

    XNOR_ENGINE static inline BuildResult m_LastProjectBuildResult = BuildResult::Unknown;

    static bool_t CheckDotnetInstalled();

    static bool_t CheckDotnetVersion();

    static void CoralMessageCallback(std::string_view message, Coral::MessageLevel level);
    
    static void CoralExceptionCallback(std::string_view message);

    static Logger::LogLevel CoralMessageLevelToXnor(Coral::MessageLevel level);
};

END_XNOR_CORE
