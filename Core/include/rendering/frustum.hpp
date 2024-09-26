#pragma once

#include <array>
#include <Maths/vector3.hpp>

#include "core.hpp"
#include "utils/plane.hpp"
#include "utils/bound.hpp"
#include "camera.hpp"

BEGIN_XNOR_CORE


class XNOR_ENGINE Frustum
{
public:
    enum Face
    {
        Top,
        Bottom,
        Right,
        Left,
        Far,
        Near,

        Size
    };

    std::array<Plane,Size> plane;


    DEFAULT_COPY_MOVE_OPERATIONS(Frustum)

    Frustum() = default;

    ~Frustum() = default;

    void UpdateFromCamera(const Camera& camera, float_t aspect);

    bool_t IsOnFrustum(const Bound& bound) const;

private:
    void UpdateCameraPerspective(const Camera& camera, float_t aspect);

    void UpdateCameraOrthoGraphic(const Camera& camera, float_t aspect);
};

END_XNOR_CORE