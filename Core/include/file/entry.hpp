﻿#pragma once

#include <filesystem>

#include "core.hpp"
#include "utils/pointer.hpp"

/// @file entry.hpp
/// @brief Defines the XnorCore::Entry class.

BEGIN_XNOR_CORE

class Directory;

/// @brief File system entry. Can be either a File or a Directory.
class Entry
{
public:
    /// @brief Constructs an Entry corresponding to the given @p path.
    [[nodiscard]]
    XNOR_ENGINE explicit Entry(std::filesystem::path&& path);

    /// @brief Default Entry destruction.
    XNOR_ENGINE virtual ~Entry() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Entry)

    /// @brief Loads the contents of this Entry.
    /// 
    /// @returns @c false if an error occured while loading.
    XNOR_ENGINE virtual bool_t Load() = 0;

    /// @brief Unloads the contents of this Entry.
    XNOR_ENGINE virtual void Unload() = 0;

    /// @brief Reloads the contents of this Entry.
    ///
    /// This if effectively the same as calling Unload() and then Load().
    XNOR_ENGINE virtual bool_t Reload();

    /// @brief Opens this Entry in the file explorer
    XNOR_ENGINE virtual void OpenInExplorer() const;

    /// @brief Returns the corresponding filesystem path.
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetPath() const;

    /// @brief Returns the string representation of GetPath.
    [[nodiscard]]
    XNOR_ENGINE std::string GetPathString() const;

    /// @brief Returns this Entry name.
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    /// @brief Renames this Entry on the file system.
    ///
    /// This function also renames the File entry in the FileManager using FileManager::Rename(const std::filesystem::path&, const std::filesystem::path&).
    /// 
    /// @param newName The new name of this Entry.
    XNOR_ENGINE virtual void SetName(const std::string& newName);

    /// @brief Returns whether this Entry has been loaded.
    [[nodiscard]]
    XNOR_ENGINE bool_t GetLoaded() const;

    /// @brief Sets the new path of this Entry.
    virtual void SetParent(Pointer<Directory>&& newParent);

    /// @brief Returns a Pointer to the parent Directory of this Entry, with a strong reference stored in the FileManager.
    [[nodiscard]]
    XNOR_ENGINE Pointer<Directory> GetParent();

protected:
    /// @brief The underlying filesystem path of this Entry.
    std::filesystem::path m_Path;
    /// @brief The name of this Entry.
    std::string m_Name;

    /// @brief Whether this Entry has been loaded.
    /// 
    /// Default implementation of Load and Unload functions in the Entry class already change this value accordingly.
    /// Any override of such function must either call the parent implementation or update this variable so that GetLoaded
    /// returns the correct state.
    bool_t m_Loaded = false;

    /// @brief The parent of this Entry.
    Pointer<Directory> m_Parent;

    /// @brief Updates fields of this class using the new value of m_Path.
    virtual void UpdateUtilityValues();
};

END_XNOR_CORE

// We cannot include directory.hpp above the class declaration, as the Directory class inherits from Entry.
// However, we also need to include this in every file in which we include entry.hpp, as the Entry class contains a Pointer<Directory>
// which needs the Directory class to be defined at some point so its destructor is defined as well
#include "file/directory.hpp"
