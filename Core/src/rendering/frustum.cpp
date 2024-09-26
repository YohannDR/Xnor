#include "rendering/frustum.hpp"

#include "resource/model.hpp"

using namespace XnorCore;

void Frustum::UpdateFromCamera(const Camera& camera, const float_t aspect)
{
    if (camera.isOrthographic)
        UpdateCameraOrthoGraphic(camera, aspect);
    else
        UpdateCameraPerspective(camera, aspect);
        
    
}

bool_t Frustum::IsOnFrustum(const Bound& bound) const
{
    const bool_t top  = bound.IsOnPlane(plane[Top]);
    const bool_t bottom  = bound.IsOnPlane(plane[Bottom]);
    const bool_t near  = bound.IsOnPlane(plane[Near]);
    const bool_t far  = bound.IsOnPlane(plane[Far]);
    const bool_t right  = bound.IsOnPlane(plane[Right]);
    const bool_t left  = bound.IsOnPlane(plane[Left]);

    

    return top && bottom && near && far && right && left;
}

void Frustum::UpdateCameraPerspective(const Camera& camera, float_t aspect)
{
    const float_t halfVSide = camera.far * tanf(camera.fov * Calc::Deg2Rad * .5f);
    const float_t halfHSide = halfVSide * aspect;
    
    const Vector3 frontMultFar = camera.far * camera.front;
    
    plane[Near] = Plane(camera.position + camera.near * camera.front, camera.front);
    
    plane[Far] = Plane(camera.position + frontMultFar, -camera.front);
    
    plane[Right] = Plane(camera.position,Vector3::Cross(frontMultFar - camera.right * halfHSide, camera.up));
    
    plane[Left] = Plane(camera.position,Vector3::Cross(camera.up,frontMultFar + (camera.right * halfHSide)));
    
    plane[Top] = Plane(camera.position, Vector3::Cross(camera.right, frontMultFar - (camera.up * halfVSide)));
    
    plane[Bottom] = Plane(camera.position, Vector3::Cross(frontMultFar + (camera.up * halfVSide), camera.right));

}   

void Frustum::UpdateCameraOrthoGraphic(const Camera& camera, float_t aspect)
{
    const float_t halfVSide = camera.far * 0.5f;
    const float_t halfHSide = halfVSide * aspect;

    const Vector3 frontMultFar = camera.far * camera.front;
    
    plane[Near] = Plane(camera.position + camera.near * camera.front, camera.front);
    
    plane[Far] = Plane(camera.position + frontMultFar, -camera.front);
    
    plane[Right] = Plane(camera.position,Vector3::Cross(frontMultFar - camera.right * halfHSide, camera.up));
    
    plane[Left] = Plane(camera.position,Vector3::Cross(camera.up,frontMultFar + (camera.right * halfHSide)));
    
    plane[Top] = Plane(camera.position, Vector3::Cross(camera.right, frontMultFar - (camera.up * halfVSide)));
    
    plane[Bottom] = Plane(camera.position, Vector3::Cross(frontMultFar + (camera.up * halfVSide), camera.right));
}
