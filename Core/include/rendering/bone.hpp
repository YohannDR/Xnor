#pragma once

#include "core.hpp"
#include "Maths/matrix.hpp"
#include "Maths/quaternion.hpp"
#include "Maths/vector3.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Bone final
{
public:
    XNOR_ENGINE Bone() = default;
    XNOR_ENGINE ~Bone() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Bone)

    XNOR_ENGINE void Create(const Matrix& localMatrix, const Matrix& offsetMatrix);

    std::string name = "Bone_Unnamed";
    int32_t id{};
    Vector3 position;
    Quaternion rotation;
    Matrix local;
    Matrix global;
    Matrix globalInverse;
    int32_t parentId = -1;
    List<int32_t> children;
};

END_XNOR_CORE
