#pragma once

#include <unordered_map>

#include "file/file.hpp"
#include "resource/resource.hpp"
#include "utils/logger.hpp"
#include "utils/pointer.hpp"

/// @file resource_manager.hpp
/// @brief Defines the XnorCore::ResourceManager class.

BEGIN_XNOR_CORE

/// @brief Static class used to add, load, get, or unload @ref XnorCore::Resource "Resources".
///
/// It contains all wrapper instances of the Resource class. These are either added or loaded using the corresponding
/// function: ResourceManager::Add and ResourceManager::Load.
class ResourceManager final
{
    STATIC_CLASS(ResourceManager)
    
public:
    /// @brief Special prefix used to store automatically added @ref Shader "Shaders".
    ///
    /// This is mainly used in LoadAll.
    static constexpr const char_t* const ReservedShaderPrefix = "_shaders/";

    /// @brief The path to the GUID map file.
    static constexpr const char_t* const GuidMapFilePath = "assets/guid_map.txt";
    
    /// @brief Creates the Resource corresponding to the given @p name without loading it.
    template <Concepts::ResourceT T>
    static Pointer<T> Add(const std::string& name);
    
    /// @brief Creates the Resource corresponding to the given @p file without loading it.
    template <Concepts::ResourceT T>
    static Pointer<T> Add(const Pointer<File>& file);

    /// @brief Creates the Resource corresponding to the given @p file and loads it.
    template <Concepts::ResourceT T>
    static Pointer<T> Load(const Pointer<File>& file, bool_t loadInRhi = true);

    /// @brief Creates one Resource for each @ref FileManager entry.
    XNOR_ENGINE static void LoadAll();

    /// @brief Loads the Guid resource map internally
    XNOR_ENGINE static void LoadGuidMap();

    /// @brief Saves the Guid resource map internally
    XNOR_ENGINE static void SaveGuidMap();

    /// @brief Checks whether the ResourceManager contains the specified Resource name.
    [[nodiscard]]
    XNOR_ENGINE static bool_t Contains(const std::string& name);

    /// @brief Checks whether the ResourceManager contains the specified Resource file name.
    [[nodiscard]]
    XNOR_ENGINE static bool_t Contains(const Pointer<File>& file);

    /// @brief Returns the Resource that was either added or loaded using the given @p name.
    template <Concepts::ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Get(const std::string& name);

    /// @brief Returns the Resource that was either added or loaded using the given file name.
    template <Concepts::ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Get(const Pointer<File>& file);

    /// @brief Returns the Resource that was either added or loaded using the given file name.
    template <Concepts::ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Get(const Guid& guid);

    /// @brief Renames the Resource with the given @p name to @p newName.
    ///
    /// @note This function only renames the key used to store this Resource, and doesn't in any case rename the Resource itself.
    /// For this exact reason, using Resource::SetName instead is the preferred way or renaming a Resource.
    XNOR_ENGINE static void Rename(const std::string& name, const std::string& newName);

    /// @brief Renames the given @p resource to @p newName.
    ///
    /// @note This function only renames the key used to store this Resource, and doesn't in any case rename the Resource itself.
    /// For this exact reason, using Resource::SetName instead is the preferred way or renaming a Resource.
    XNOR_ENGINE static void Rename(const Pointer<Resource>& resource, const std::string& newName);

    /// @brief Finds all Resource of type @p T.
    /// @tparam T The type of Resource to find.
    /// @return All stored Resource of type @p T.
    template <Concepts::ResourceT T>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll();
    
    /// @see ResourceManager::FindAll()
    template <Concepts::ResourceT T>
    static void FindAll(std::vector<Pointer<T>>* result);

    /// @brief Finds a specific Resource based on a predicate.
    /// @tparam T The type of Resource to find.
    /// @param predicate The predicate used to find the correct Resource. This function will be
    /// called for each stored Resource.
    /// @return The first Resource for which the @p predicate returned @c true. If every Resource
    /// returned @c false, instead return a null @ref Pointer.
    template <Concepts::ResourceT T = Resource>
    [[nodiscard]]
    static Pointer<T> Find(std::function<bool_t(Pointer<T>)>&& predicate);
    
    /// @brief Finds a list of Resource based on a predicate.
    /// @tparam T The type of Resource to find.
    /// @param predicate The predicate used to find the correct Resource. This function will be
    /// called for each stored Resource.
    /// @return The first Resource for which the @p predicate returned @c true. If every Resource
    /// returned @c false, instead return a null @ref Pointer.
    template <Concepts::ResourceT T = Resource>
    [[nodiscard]]
    static std::vector<Pointer<T>> FindAll(std::function<bool_t(Pointer<T>)>&& predicate);

    /// @see @ref FileManager::FindAll(std::function<bool_t(Pointer<T>)>&&)
    template <Concepts::ResourceT T>
    static void FindAll(std::function<bool_t(Pointer<T>)>&& predicate, std::vector<Pointer<T>>* result);

    /// @brief Checks whether the given @p name corresponds to a Resource of type @p T.
    template <Concepts::ResourceT T>
    [[nodiscard]]
    static bool_t IsResourceOfType(const std::string& name);

    /// @brief Unloads the Resource with the given @p name.
    XNOR_ENGINE static void Unload(const std::string& name);

    /// @brief Unloads the given @p resource.
    template <Concepts::ResourceT T>
    static void Unload(const Pointer<T>& resource);

    /// @brief Unloads all stored @ref XnorCore::Resource "Resources".
    XNOR_ENGINE static void UnloadAll();

private:
    XNOR_ENGINE static inline std::unordered_map<std::string, Pointer<Resource>> m_Resources;
    XNOR_ENGINE static inline std::mutex m_ResourcesMutex;
    XNOR_ENGINE static inline std::unordered_map<Guid, std::string> m_GuidMap;
    
    template <Concepts::ResourceT T>
    static Pointer<T> AddNoCheck(std::string name);
    
    template <Concepts::ResourceT T>
    static Pointer<T> LoadNoCheck(Pointer<File> file, bool_t loadInRhi = true);

    template <Concepts::ResourceT T>
    [[nodiscard]]
    static Pointer<T> GetNoCheck(const std::string& name);
};

END_XNOR_CORE

#include "resource/resource_manager.inl"
