#pragma once

#include <vector>

#include "core.hpp"
#include "Maths/matrix.hpp"
#include "Maths/vector4.hpp"
#include "rendering/camera.hpp"
#include "rendering/rhi_typedef.hpp"
#include "utils/bound.hpp"

BEGIN_XNOR_CORE
class CascadeShadowMap
{
public:
    XNOR_ENGINE CascadeShadowMap() = default;

    XNOR_ENGINE ~CascadeShadowMap() = default;
    
    DEFAULT_COPY_MOVE_OPERATIONS(CascadeShadowMap)

    XNOR_ENGINE void GetCascadeCameras(std::vector<Camera>* cameras ,const Camera&  viewPortCamera, Vector3 lightDir, Vector2i screenSize);
    
    XNOR_ENGINE void SetCascadeLevel(const std::array<float, DirectionalCascadeLevel>& cascadeLevel);

    XNOR_ENGINE void SetZMultiplicator(const float_t zMultiPlicator);

private:
    std::array<float_t,DirectionalCascadeLevel> m_CascadeLevel;
    
    float_t m_ZMultiplicator = 10.f;
    
    XNOR_ENGINE void ComputeFrustumCorner(std::vector<Vector4>* frustumCornerWorldSpace, const Matrix& proj, const Matrix& view);

    XNOR_ENGINE void GetCamera(Camera* cascadedCamera, float_t cascadedNear, float_t cascadedFar, const Camera& baseCamera, Vector3 lightDir, Vector2i screenSize);


};

END_XNOR_CORE