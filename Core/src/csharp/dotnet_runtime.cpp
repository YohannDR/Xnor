#include "csharp/dotnet_runtime.hpp"

#include <cstdio>

#include "file/file.hpp"

#include "application.hpp"
#include "Coral/GC.hpp"
#include "csharp/dotnet_constants.hpp"
#include "reflection/dotnet_reflection.hpp"
#include "utils/message_box.hpp"
#include "utils/windows.hpp"

using namespace XnorCore;

constexpr const char_t* AlcName = "XNOR .NET AssemblyLoadContext";

Coral::HostSettings DotnetRuntime::m_Settings =
{
    .MessageCallback = CoralMessageCallback,
    .MessageFilter = Coral::MessageLevel::Warning,
    .ExceptionCallback = CoralExceptionCallback
};

bool_t DotnetRuntime::Initialize()
{
    Logger::LogInfo("Initializing .NET runtime");

    // Set .NET commands result language to English
    SetEnvironmentVariableA("DOTNET_CLI_UI_LANGUAGE", "en");

    if (!CheckDotnetInstalled())
    {
        constexpr const char_t* const errorMessage = ".NET is not installed on this machine";
        Logger::LogFatal(errorMessage);
        MessageBox::Show(errorMessage, "Fatal Error", MessageBox::Type::Ok, MessageBox::Icon::Error);
        throw std::runtime_error(".NET is not installed on this machine");
    }

    if (!CheckDotnetVersion())
    {
        const std::string errorMessage = std::format("Invalid .NET version. XNOR Engine needs .NET {}.0", DotnetVersion);
        Logger::LogFatal(errorMessage);
        MessageBox::Show(errorMessage, "Fatal Error", MessageBox::Type::Ok, MessageBox::Icon::Error);
        throw std::runtime_error("Invalid .NET version");
    }

    m_AssembliesPath = Application::executablePath.parent_path() / Dotnet::AssembliesDirectory;
    m_Settings.CoralDirectory = m_AssembliesPath.string();
    
    if (!m_Runtime.Initialize(m_Settings))
    {
        Logger::LogError("An unknown error occured while initializing .NET runtime");
        return false;
    }

    m_Alc = m_Runtime.CreateAssemblyLoadContext(AlcName);

    if (!LoadAssembly(Dotnet::CoreProjectName))
    {
        Logger::LogError("An unknown error occured while loading XNOR .NET library");
        return false;
    }

    return true;
}

void DotnetRuntime::Shutdown()
{
    Logger::LogInfo("Shutting down .NET runtime");
    
    if (!m_LoadedAssemblies.empty())
        UnloadAllAssemblies();
    if (m_Initialized)
        m_Runtime.Shutdown();
}

bool_t DotnetRuntime::LoadAssembly(const std::string& name)
{
    const std::filesystem::path&& filepath = m_AssembliesPath / (name + ".dll");
    
    Logger::LogInfo("Loading .NET assembly {}", filepath.filename());

    const std::string&& str = filepath.string();
    
    DotnetAssembly* const assembly = new DotnetAssembly(str);
    if (assembly->Load(m_Alc))
    {
        m_LoadedAssemblies.push_back(assembly);
        assembly->ProcessTypes();
        return true;
    }
    
    return false;
}

DotnetAssembly* DotnetRuntime::GetAssembly(const std::string& name)
{
    for (auto&& assembly : m_LoadedAssemblies)
    {
        if (assembly->GetName() == name)
            return assembly;
    }

    return nullptr;
}

void DotnetRuntime::GcCollect(bool_t wait)
{
    Coral::GC::Collect();
    
    if (wait)
        Coral::GC::WaitForPendingFinalizers();
}

DotnetAssembly* DotnetRuntime::GetGameAssembly() { return GetAssembly(Dotnet::GameProjectName); }

void DotnetRuntime::UnloadAllAssemblies(const bool_t reloadContext)
{
    Logger::LogInfo("Unloading {} .NET assemblies", m_LoadedAssemblies.size());
    
    GcCollect();
    
    for (auto&& assembly : m_LoadedAssemblies)
        delete assembly;
    m_LoadedAssemblies.clear();
    
    m_Runtime.UnloadAssemblyLoadContext(m_Alc);

    if (reloadContext)
        m_Alc = m_Runtime.CreateAssemblyLoadContext(AlcName);
}

void DotnetRuntime::ReloadAllAssemblies()
{
    std::vector<std::string> assemblies;
    std::ranges::transform(m_LoadedAssemblies, std::back_inserter(assemblies), [](const decltype(m_LoadedAssemblies)::value_type& loadedAssembly) { return loadedAssembly->GetName(); });

    m_ProjectReloadingProgress += 0.05f;
    
    UnloadAllAssemblies(true);

    m_ProjectReloadingProgress += 0.05f;
    
    const float_t remainingProgress = 1.f - m_ProjectReloadingProgress;
    const size_t assemblyCount = assemblies.size();
    
    for (auto&& assembly : assemblies)
    {
        const bool_t loadResult = LoadAssembly(assembly);

        m_ProjectReloadingProgress += remainingProgress / static_cast<float_t>(assemblyCount);
        
        if (loadResult)
            continue;

        Logger::LogWarning("Couldn't reload assembly {}", assembly);
    }
}

bool_t DotnetRuntime::BuildGameProject(const bool_t asynchronous)
{
    const std::filesystem::path gameProjectDirectory = Dotnet::GameProjectLocation;

    if (!exists(gameProjectDirectory))
        return false;

    if (!is_directory(gameProjectDirectory))
        return false;

    if (!exists(gameProjectDirectory / std::format("{}.csproj", Dotnet::GameProjectName)))
        return false;

    Logger::LogInfo("Building Game project");

    static constexpr const char_t* const TempFile = "xnor_dotnet_build.txt";
    const std::filesystem::path tempPath = std::filesystem::temp_directory_path() / TempFile;

    m_ProjectReloadingProgress = 0.2f;
    
    m_LastProjectBuildResult = BuildResult::Unknown;

    Utils::TerminalCommand(std::string("dotnet clean ")+ " \"" + absolute(gameProjectDirectory).string() + '"');

    const int32_t commandResult = Utils::TerminalCommand(std::string("dotnet build ") + Dotnet::GameProjectBuildOptions + " \"" + absolute(gameProjectDirectory).string() + "\" 1> \"" + tempPath.string() + '"', asynchronous);

    m_ProjectReloadingProgress = 0.5f;

    if (commandResult == 0)
        m_LastProjectBuildResult = BuildResult::Success;

    // In case a warning/error occured, read the output file to understand what happened
    std::ifstream file(tempPath);
    std::string line;

    // Dotnet outputs look like this:
    // <MSBuild version>
    //   <Projects being restored>
    //   <Projects being built>
    //
    // Build <succeeded/FAILED>
    //
    // <Warning list>
    // <Error list>
    //     <Total warning count>
    //     <Total error count>
    //
    // <Time elapsed>
    
    while (!line.starts_with("Build succeeded") && !line.starts_with("Build FAILED"))
        std::getline(file, line);

    // Here we are right after the 'Build <succeeded/FAILED>' line

    std::getline(file, line);

    // This is the first potential warning/error line
    std::getline(file, line);

    std::vector<std::string> diagnostics;
    while (!line.starts_with("    "))
    {
        diagnostics.push_back(line);
        std::getline(file, line);
    }

    if (!diagnostics.empty())
    {
        for (auto&& str : diagnostics)
        {
            if (str.find(": warning") != std::string::npos)
            {
                m_LastProjectBuildResult = BuildResult::Warning;
                Logger::LogWarning("[.NET BUILD] {}", str);
            }
            else
            {
                m_LastProjectBuildResult = BuildResult::Error;
                Logger::LogError("[.NET BUILD] {}", str);
            }
        }
    }

    file.close();

    std::filesystem::remove(tempPath);

    if (commandResult == 0)
    {
        m_ProjectReloadingProgress = 0.6f;

        Logger::LogInfo("Build succeeded");
        return true;
    }

    m_ProjectReloadingProgress = 1.f;
    
    Logger::LogError("Build failed");
    return false;
}

void DotnetRuntime::BuildAndReloadProject(const bool_t recreateScriptInstances)
{
    m_ReloadingProject = true;
    m_ProjectReloadingProgress = 0.f;

    std::vector<ScriptComponent*> scripts;
    World::scene->GetAllComponentsOfType(&scripts);
    std::vector<std::pair<Entity*, std::string>> managedTypeEntityPairs(scripts.size());

    for (size_t i = 0; i < scripts.size(); i++)
    {
        ScriptComponent* const script = scripts[i];
        auto&& pair = std::make_pair(script->GetEntity(), script->m_ManagedObject.GetType().GetFullName());
        if (recreateScriptInstances)
            managedTypeEntityPairs[i] = pair;
        pair.first->RemoveComponent<ScriptComponent>();
    }
    
    m_ProjectReloadingProgress = 0.1f;
    
    if (BuildGameProject(false))
    {
        ReloadAllAssemblies();

        m_ProjectReloadingProgress = 0.9f;

        if (recreateScriptInstances)
        {
            auto&& gameTypes = GetGameAssembly()->GetCoralAssembly()->GetTypes();
            for (size_t i = 0; i < scripts.size(); i++)
            {
                auto&& pair = managedTypeEntityPairs[i];
                auto&& it = std::ranges::find_if(gameTypes, [&](const Coral::Type* const type) -> bool_t { return type->GetFullName() == pair.second; });
                if (it != gameTypes.end())
                    pair.first->AddComponent(ScriptComponent::New(pair.second, GetGameAssembly()));
            }
        }
    }
    else
    {
        Logger::LogError("Couldn't build {} .NET project", Dotnet::GameProjectName);
    }
    
    m_ProjectReloadingProgress = 1.f;
    m_ReloadingProject = false;
}

bool_t DotnetRuntime::GetInitialized() { return m_Initialized; }

bool_t DotnetRuntime::IsReloadingProject() { return m_ReloadingProject; }

float_t DotnetRuntime::GetProjectReloadingProgress() { return m_ProjectReloadingProgress; }

DotnetRuntime::BuildResult DotnetRuntime::GetProjectLastBuildResult() { return m_LastProjectBuildResult; }

bool_t DotnetRuntime::CheckDotnetInstalled()
{
    // Check if the dotnet command returns a non-zero exit code
    return Utils::TerminalCommand("dotnet --info 1> nul", false) == 0;
}

bool_t DotnetRuntime::CheckDotnetVersion()
{
    // This function runs the 'dotnet --list-runtimes' command
    // This prints a list of all installed .NET runtimes on the current machine
    // We redirect the command output to TEMP_FILE_PATH and read it line by line
    // to find one that suits us, e.g. one whose version is equal to the DotnetVersion constant
    // Once this is done, we know for sure that the C# assemblies can be executed and let
    // the system choose the right version

    static constexpr const char_t* const TempFile = "xnor_dotnet_list_runtimes.txt";
    const std::filesystem::path tempPath = std::filesystem::temp_directory_path() / TempFile;
    
    Utils::TerminalCommand("dotnet --list-runtimes 1> \"" + tempPath.string() + '"', false);
    
    File file(tempPath.string());
    
    file.Load();

    constexpr const char_t* dotnetCoreName = "Microsoft.NETCore.App";
    const size_t dotnetCoreNameLength = std::strlen(dotnetCoreName);

    const char_t* const data = file.GetData();
    std::stringstream stream(data);
    std::string line;
    bool_t foundValidDotnet = false;
    while (!stream.eof())
    {
        std::getline(stream, line);

        if (!line.starts_with(dotnetCoreName))
            continue;

        std::string sub = line.substr(dotnetCoreNameLength + 1);
        int32_t major = 0, minor = 0;
        (void) sscanf_s(sub.c_str(), "%d.%d", &major, &minor);
        
        if (major == DotnetVersion)
        {
            foundValidDotnet = true;
            break;
        }
    }
    
    file.Unload();
    std::filesystem::remove(tempPath);
    
    return foundValidDotnet;
}

void DotnetRuntime::CoralMessageCallback(std::string_view message, const Coral::MessageLevel level)
{
    Logger::Log(CoralMessageLevelToXnor(level), "[.NET] {}", message);
}

void DotnetRuntime::CoralExceptionCallback(std::string_view message)
{
    Logger::LogError("Unhandled .NET exception: {}", message);
}

Logger::LogLevel DotnetRuntime::CoralMessageLevelToXnor(const Coral::MessageLevel level)
{
    switch (level)
    {
        case Coral::MessageLevel::Info:
            return Logger::LogLevel::Info;

        case Coral::MessageLevel::Warning:
            return Logger::LogLevel::Warning;

        case Coral::MessageLevel::Error:
        case Coral::MessageLevel::All:
            return Logger::LogLevel::Error;
    }

    // This should never happen
    throw std::runtime_error("Invalid Coral MessageLevel");
}
