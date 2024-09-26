#pragma once

#include <array>

#include <assimp/mesh.h>

#include "core.hpp"
#include "file/file.hpp"
#include "refl/refl.hpp"
#include "rendering/vertex.hpp"
#include "resource/resource.hpp"
#include "utils/bound.hpp"

/// @file model.hpp
/// @brief Defines the XnorCore::Model class.

BEGIN_XNOR_CORE

/// @brief Holds the necessary information to draw a 3D model.
class Model final : public Resource
{
public:
    /*
    /// @brief Allowed extensions for models.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 56> FileExtensions
    {
        ".3d",
        ".3ds",
        ".3mf",
        ".ac",
        ".ac3d",
        ".acc",
        ".amj",
        ".ase",
        ".ask",
        ".b3d",
        ".bvh",
        ".csm",
        ".cob",
        ".dxf",
        ".enff",
        ".gltf",
        ".glb",
        ".hmb",
        ".ifc",
        ".irr",
        ".irrmesh",
        ".lwo",
        ".lws",
        ".lxo",
        ".m3d",
        ".md2",
        ".md3",
        ".md5",
        ".mdc",
        ".mdl",
        ".mesh",
        ".mot",
        ".ms3d",
        ".ndo",
        ".nff",
        ".obj",
        ".off",
        ".ogex",
        ".ply",
        ".pmx",
        ".prj",
        ".q3o",
        ".q3s",
        ".raw",
        ".scn",
        ".sib",
        ".smd",
        ".stp",
        ".stl",
        ".ter",
        ".uc",
        ".vta",
        ".x",
        ".x3d",
        ".xgl",
        ".zgl"
    };*/
    Bound aabb;
    
    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;

    DEFAULT_COPY_MOVE_OPERATIONS(Model)

    /// @brief Destroys this Model.
    XNOR_ENGINE ~Model() override;

    /// @brief Loads a Model from assimp loaded data.
    XNOR_ENGINE bool_t Load(const aiMesh& loadedData);

    /// @copydoc XnorCore::Resource::CreateInInterface
    XNOR_ENGINE void CreateInInterface() override;

    /// @copydoc XnorCore::Resource::DestroyInInterface
    XNOR_ENGINE void DestroyInInterface() override;
    
    /// @copydoc XnorCore::Resource::Unload
    XNOR_ENGINE void Unload() override;

    XNOR_ENGINE bool_t Save() const override;

    /// @brief Gets the id of the model
    /// @return Model id
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;

#ifndef SWIG
    /// @brief Gets the vertices of the model
    /// @return Vertices
    [[nodiscard]]
    const std::vector<Vertex>& GetVertices() const;
#endif
    
private:
    XNOR_ENGINE void ComputeAabb(const aiAABB& assimpAabb);
    
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
    
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Model, bases<XnorCore::Resource>))
