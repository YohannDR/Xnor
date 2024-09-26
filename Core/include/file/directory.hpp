#pragma once

#include <filesystem>

#include "core.hpp"
#include "file/entry.hpp"
#include "utils/pointer.hpp"

/// @file directory.hpp
/// @brief Defines the XnorCore::Directory class.

BEGIN_XNOR_CORE

class File;

/// @brief Defines a directory on the filesystem.
///
/// This is meant to be used with @ref XnorCore::Pointer "Pointers" and with the FileManager.
class Directory : public Entry
{
public:
    /// @brief Constructs a Directory corresponding to the given @p path.
    [[nodiscard]]
    XNOR_ENGINE explicit Directory(std::filesystem::path&& filepath);

    /// @brief Default Directory destruction.
    XNOR_ENGINE ~Directory() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Directory)

    /// @brief Loads the contents of this Directory in the FileManager.
    ///
    /// This effectively means loading all children of this Directory.
    /// 
    /// @returns @c false if an error occured while loading.
    XNOR_ENGINE bool_t Load() override;

    /// @brief Unloads the contents of this Directory.
    XNOR_ENGINE void Unload() override;

    /// @brief Opens this Directory in the file explorer
    XNOR_ENGINE void OpenInExplorer() const override;

    /// @brief Returns all the child @ref Entry "Entries" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<Entry>>& GetChildEntries() const;

#ifndef SWIG
    /// @brief Returns all the child @ref Entry "Entries" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<Entry>>& GetChildEntries();
#endif
    
    /// @brief Returns all the child @ref File "Files" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<File>>& GetChildFiles() const;
    
#ifndef SWIG
    /// @brief Returns all the child @ref File "Files" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<File>>& GetChildFiles();
#endif
    
    /// @brief Returns all the child @ref Directory "Directories" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE const std::vector<Pointer<Directory>>& GetChildDirectories() const;
    
#ifndef SWIG
    /// @brief Returns all the child @ref Directory "Directories" of this Directory.
    [[nodiscard]]
    XNOR_ENGINE std::vector<Pointer<Directory>>& GetChildDirectories();
#endif

    /// @brief Sets the name of this Directory.
    ///
    /// This effectively means changing the path of all its children accordingly.
    void SetName(const std::string& newName) override;

private:
    std::vector<Pointer<Entry>> m_ChildEntries;
    std::vector<Pointer<File>> m_ChildFiles;
    std::vector<Pointer<Directory>> m_ChildDirectories;
};

END_XNOR_CORE

// Voluntary include after the class definition because using Pointer<File> means we need to include File at some point
#include "file/file.hpp"
