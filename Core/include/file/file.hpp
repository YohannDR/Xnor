#pragma once

#include <filesystem>

#include "core.hpp"
#include "file/entry.hpp"
#include "resource/resource.hpp"
#include "utils/pointer.hpp"

/// @file file.hpp
/// @brief Defines the XnorCore::File class.

BEGIN_XNOR_CORE

/// @brief Defines a file on the filesystem.
///
/// This is meant to be used with @ref XnorCore::Pointer "Pointers" and with the FileManager.
class File : public Entry
{
public:
    /// @brief Type of file according to file extension.
    enum class Type
    {
        Unknown,
        Texture,
        Model,
        Mesh,
        Skeleton,
        Font,
        Xml,
        AnimationMontage,
        VertexShader,
        FragmentShader,
        GeometryShader,
        ComputeShader
    };
    
    /// @brief Constructs a File corresponding to the given @p path.
    [[nodiscard]]
    XNOR_ENGINE explicit File(std::filesystem::path&& filepath);

    /// @brief Destructs the File instance by calling Unload.
    XNOR_ENGINE ~File() override;

    DEFAULT_COPY_MOVE_OPERATIONS(File)

    /// @brief Loads the contents of this File.
    /// 
    /// @returns @c false if an error occured while loading.
    XNOR_ENGINE bool_t Load() override;

    /// @brief Unloads the contents of this File.
    XNOR_ENGINE void Unload() override;

    /// @brief Opens this File in the file explorer
    XNOR_ENGINE void OpenInExplorer() const override;

    /// @brief Opens this File on the user's computer using the default software for its file extension
    XNOR_ENGINE void OpenFile() const;

    /// @brief Returns the Type of this File.
    XNOR_ENGINE Type GetType() const;

    /// @brief Deletes the corresponding filesystem file.
    XNOR_ENGINE void Delete() const;

    /// @brief Returns the name of this File without the file extension.
    [[nodiscard]]
    XNOR_ENGINE std::string GetNameNoExtension() const;

    /// @brief Returns the name of this File without the file extension.
    [[nodiscard]]
    XNOR_ENGINE std::string GetPathNoExtension() const;

    /// @brief Returns the file extension of this File.
    [[nodiscard]]
    XNOR_ENGINE std::string GetExtension() const;

    /// @brief Returns a @c const pointer to the raw loaded data.
    template <typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    /// @brief Returns a pointer to the raw loaded data.
    template <typename T = char_t>
    [[nodiscard]]
    T* GetData();

    /// @brief Returns the size of the loaded data.
    [[nodiscard]]
    XNOR_ENGINE int64_t GetSize() const;

    /// @brief Sets the name of this File.
    XNOR_ENGINE void SetName(const std::string& newName) override;

    /// @brief Returns the stored Pointer to the Resource loaded from this File.
    ///
    /// Note that a Resource doesn't need to be loaded from a File and can instead be loaded from raw data.
    /// This implies that the return value of this function can be a @c nullptr.
    [[nodiscard]]
    XNOR_ENGINE Pointer<Resource> GetResource() const;
    
protected:
    void UpdateUtilityValues() override;
    
private:
    std::string m_NameNoExtension;
    std::string m_Extension;
    std::string m_PathNoExtension;
    Type m_Type = Type::Unknown;
    
    int8_t* m_Data = nullptr;
    int64_t m_Size = 0;

    // Null if the file isn't linked to a specific resource
    Pointer<Resource> m_Resource;

    // We need this in order to set m_Resource from the ResourceManager
    // which is the only class that needs to modify this field
    friend class ResourceManager;
};

END_XNOR_CORE

// Voluntary include after the class definition because using Pointer<File> means we need to include File at some point
#include "file/file.inl"
