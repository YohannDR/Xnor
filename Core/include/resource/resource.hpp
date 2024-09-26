#pragma once

#include "core.hpp"
#include "utils/guid.hpp"
#include "utils/pointer.hpp"
#include "reflection/reflection.hpp"

BEGIN_XNOR_CORE

/// @file resource.hpp
/// @brief Defines the XnorCore::Resource class

class File;

/// @brief Interface for resources, which encapsulates most objects used in the engine that come from a file
class Resource
{
public:
    /// @brief Constructs a default Resource.
    XNOR_ENGINE Resource() = default;

    /// @brief Creates a Resource with the given @p name.
    XNOR_ENGINE explicit Resource(std::string name);

    XNOR_ENGINE virtual ~Resource() = 0;

    DEFAULT_COPY_MOVE_OPERATIONS(Resource)

    /// @brief Loads data from memory.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const uint8_t* buffer, int64_t length);

    /// @brief Loads data from file.
    ///
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Load(const Pointer<File>& file);

    /// @brief Creates the Resource in the current interface (Rhi/Audio).
    XNOR_ENGINE virtual void CreateInInterface();

    /// @brief Destroys the Resource in the current interface (Rhi/Audio).
    XNOR_ENGINE virtual void DestroyInInterface();

    /// @brief Unloads the loaded data.
    XNOR_ENGINE virtual void Unload();

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then @ref Load(const uint8_t* buffer, int64_t length) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const uint8_t* buffer, int64_t length, bool_t reloadInInterface = true);

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then @ref Load(const Pointer<File>&) "Load".
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(const Pointer<File>& file, bool_t reloadInInterface = true);

    /// @brief Unloads and then loads back this Resource.
    ///
    /// This is effectively equivalent to calling Unload and then Load(const Pointer<File>&)
    /// using FileManager::Get(const std::filesystem::path&) as a parameter.
    /// 
    /// @returns @c true if the loading succeeded, @c false otherwise.
    XNOR_ENGINE virtual bool_t Reload(bool_t reloadInInterface = true);

    XNOR_ENGINE virtual bool_t Save() const;

    /// @brief Returns whether the Resource has already been loaded.
    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoaded() const;

    /// @brief Returns whether the Resource has already been loaded in the associated interface (Rhi/Audio).
    [[nodiscard]]
    XNOR_ENGINE bool_t IsLoadedInInterface() const;

    /// @brief Returns the name of this Resource.
    [[nodiscard]]
    XNOR_ENGINE std::string GetName() const;

    /// @brief Sets the name of this Resource.
    ///
    /// This function also renames the Resource entry in the ResourceManager using ResourceManager::Rename(const std::string&, const std::string&).
    XNOR_ENGINE void SetName(std::string newName);

    /// @brief Sets the GUID of this Resource.
    XNOR_ENGINE void SetGuid(const Guid& guid);

    /// @brief Returns the Guid of this Resource.
    [[nodiscard]]
    XNOR_ENGINE const Guid& GetGuid() const;

    XNOR_ENGINE void SetFile(const Pointer<File> file);

protected:
    /// @brief Whether the resource was loaded
    bool_t m_Loaded = false;
    /// @brief Whether the resource was loaded in the associated interface (Rhi/Audio)
    bool_t m_LoadedInInterface = false;

    /// @brief Name of the resource
    std::string m_Name;
    /// @brief Guid of the resource
    Guid m_Guid;

    Pointer<File> m_File;

    // We need this in order to set m_File from the ResourceManager
    // which is the only class that needs to modify this field
    friend class ResourceManager;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Resource))

#include "file/file.hpp"
