#pragma once

#include <array>

#include <assimp/mesh.h>

#include "core.hpp"
#include "assimp/scene.h"
#include "file/file.hpp"
#include "rendering/bone.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Mesh;

class Skeleton final : public Resource
{
public:    
    /// @brief Allowed extensions for skeletons.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FileExtensions
    {
        ".max",
        ".xsi",
        ".ma"
    };

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Skeleton)

    XNOR_ENGINE ~Skeleton() override = default;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    XNOR_ENGINE bool_t Load(const aiSkeleton& loadedData);
    XNOR_ENGINE bool_t Load(const aiMesh& loadedData, const aiNode& rootNode);
    XNOR_ENGINE bool_t Load(const aiScene& scene, const aiAnimation& loadedData);

    /// @brief Re-orders how the bones are stored in order to have the parents first and the children after
    XNOR_ENGINE void ReorderBones();

    [[nodiscard]]
    XNOR_ENGINE const List<Bone>& GetBones() const;

    Mesh* mesh = nullptr;

private:
    List<Bone> m_Bones;

    XNOR_ENGINE void PrintBones() const;

    XNOR_ENGINE void ReorderBones(Bone* parent, List<Bone>* newBones);
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Skeleton, bases<XnorCore::Resource>))
