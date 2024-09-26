#pragma once

#include <filesystem>
#include <map>

#include "core.hpp"
#include "file/directory.hpp"
#include "file/file.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

/// @file file_manager.hpp
/// @brief Defines the XnorCore::FileManager class.

BEGIN_XNOR_CORE

/// @brief Static class used to add, load, get, or unload @ref File "Files" and @ref Directory "Directories".
///
/// It contains all wrapper instances of the File and Directory classes. These are either added or loaded using the corresponding
/// function: FileManager::Add and FileManager::Load for @ref File "Files",and FileManager::AddDirectory and
/// FileManager::LoadDirectory for @ref Directory "Directories".
class FileManager final
{
    STATIC_CLASS(FileManager)
    
public:
    /// @brief Creates the File corresponding to the given @p path without loading it.
    XNOR_ENGINE static Pointer<File> Add(std::filesystem::path path);
    
    /// @brief Creates the File corresponding to the given @p path and loads it.
    XNOR_ENGINE static Pointer<File> Load(std::filesystem::path path);
    
    /// @brief Creates a Directory corresponding to the given @p path without loading it.
    XNOR_ENGINE static Pointer<Directory> AddDirectory(std::filesystem::path path);
    
    /// @brief Creates the Directory corresponding to the given @p path and loads it.
    /// @see Directory::Load
    XNOR_ENGINE static Pointer<Directory> LoadDirectory(std::filesystem::path path);

    /// @brief Checks whether the FileManager contains the specified Entry path.
    [[nodiscard]]
    XNOR_ENGINE static bool_t Contains(const std::filesystem::path& path);

    /// @brief Tries to get the Entry with the given @p path.
    /// @tparam T The type of Entry to get.
    /// @param path The system path to get the Entry of.
    /// @return The Entry corresponding to the given @p path, or nullptr if it hasn't been added yet.
    template <Concepts::EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Get(const std::filesystem::path& path);

    /// @brief Renames the Entry with the specified path to another path.
    ///
    /// @note This function only renames the key used to store this Entry, and doesn't in any case rename the Entry itself.
    /// For this exact reason, using Entry::SetName instead is the preferred way or renaming a Entry.
    XNOR_ENGINE static void Rename(const std::filesystem::path& path, const std::filesystem::path& newPath);

    /// @brief Renames the specified Entry to another path.
    ///
    /// @note This function only renames the key used to store this Entry, and doesn't in any case rename the Entry itself.
    /// For this exact reason, using Entry::SetName instead is the preferred way or renaming a Entry.
    XNOR_ENGINE static void Rename(const Pointer<Entry>& entry, const std::filesystem::path& newPath);

    /// @brief Finds the first Entry of type @p T.
    /// @tparam T The type of Entry to find.
    /// @return The first Entry of type @p T.
    template <Concepts::EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Find();

    /// @brief Finds a specific Entry based on a predicate.
    /// @tparam T The type of Entry to find.
    /// @param predicate The predicate used to find the correct Entry. This function will be
    /// called for each stored Entry.
    /// @return The first Entry for which the @p predicate returned @c true. If every Entry
    /// returned @c false, instead return a null Pointer.
    template <Concepts::EntryT T = File>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @brief Finds all @ref Entry "Entries" of type @p T.
    /// @tparam T The type of Entry to find.
    /// @return All stored @ref Entry "Entries" of type @p T.
    template <Concepts::EntryT T = File>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll();
    
    /// @see FileManager::FindAll()
    template <Concepts::EntryT T = File>
    static void FindAll(std::vector<Pointer<T>>* result);
    
    /// @brief Finds a list of Entry "Entries" based on a predicate.
    /// @tparam T The type of Entry to find.
    /// @param predicate The predicate used to find the correct Entry. This function will be
    /// called for each stored Entry.
    /// @return The first Entry for which the @p predicate returned @c true. If every Entry
    /// returned @c false, instead return a null Pointer.
    template <Concepts::EntryT T = File>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @see FileManager::FindAll(std::function<bool(Pointer<T>)>&&)
    template <Concepts::EntryT T = File>
    static void FindAll(std::function<bool(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

    /// @brief Unloads the Entry corresponding to the given path.
    XNOR_ENGINE static void Unload(const std::filesystem::path& path);

    /// @brief Unloads the given Entry.
    XNOR_ENGINE static void Unload(const Pointer<Entry>& entry);

    /// @brief Unloads all stored entries.
    XNOR_ENGINE static void UnloadAll();
    
private:
    XNOR_ENGINE static inline std::map<std::filesystem::path, Pointer<Entry>> m_Entries;
};

END_XNOR_CORE

#include "file/file_manager.inl"
