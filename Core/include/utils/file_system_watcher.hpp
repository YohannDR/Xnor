#pragma once

#include <chrono>

#include "core.hpp"
#include "utils/event.hpp"

// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppEnforceTypeAliasCodeStyle
typedef unsigned long DWORD; // Windows type forward declaration  // NOLINT(modernize-use-using)

BEGIN_XNOR_CORE

/// @private
struct FswEventArgs
{
    std::filesystem::path path;
};

/// @private
struct FswRenamedEventArgs : FswEventArgs
{
    std::filesystem::path oldPath;
};

/// @private
BEGIN_ENUM(FswNotifyFilters)
{
    FileName        = 1 << 0,
    DirectoryName   = 1 << 1,
    Attributes      = 1 << 2,
    Size            = 1 << 3,
    LastWrite       = 1 << 4,
    LastAccess      = 1 << 5,
    Creation        = 1 << 6,
    Security        = 1 << 7,

    Default         = Creation | FileName | DirectoryName | LastWrite,

    All             = FileName | DirectoryName | Attributes | Size | LastWrite | LastAccess | Creation | Security
}
END_ENUM

/// @private
class FileSystemWatcher
{
public:
    Event<const FswEventArgs&> onModified;
    Event<const FswEventArgs&> onCreated;
    Event<const FswEventArgs&> onDeleted;
    Event<const FswRenamedEventArgs&> onRenamed;

    /// @brief Time between each update.
    std::chrono::milliseconds updateRate{750};

    /// @brief Whether to check the directory contents. Doesn't do anything if the watched path points to a file.
    bool_t checkContents = true;

    /// @brief Whether to check subdirectories. Doesn't do anything if the watched path points to a file or if @c checkContents is @c false
    bool_t recursive = false;

    /// @brief File extensions including the dot '.'
    List<std::string> fileExtensions;

    ENUM_VALUE(FswNotifyFilters) notifyFilters = FswNotifyFilters::Default;
    
    XNOR_ENGINE explicit FileSystemWatcher(const std::string& path);

    XNOR_ENGINE ~FileSystemWatcher();

    DELETE_COPY_MOVE_OPERATIONS(FileSystemWatcher)

    XNOR_ENGINE void Start();

    XNOR_ENGINE void Stop();

    /// @brief Forces the watcher thread wake up and update now.
    XNOR_ENGINE void Update();

    XNOR_ENGINE std::filesystem::path GetPath() const;

    XNOR_ENGINE void SetPath(const std::filesystem::path& newPath);

private:
    std::thread m_Thread;
    std::condition_variable m_CondVar;
    std::mutex m_Mutex;
    
    std::filesystem::path m_Path;
    bool_t m_IsDirectory = false;

    bool_t m_Running = false;

    bool_t m_PathChanged = false;

    void Run();

    static DWORD NotifyFiltersToWindows(ENUM_VALUE(FswNotifyFilters) filters);
};

END_XNOR_CORE
