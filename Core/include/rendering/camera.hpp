#pragma once

#include "core.hpp"
#include "rhi_typedef.hpp"

#include "Maths/vector2i.hpp"
#include "Maths/vector3.hpp"

/// @file camera.hpp
/// @brief Defines the XnorCore::Camera class.

BEGIN_XNOR_CORE

/// @brief Defines a camera
class XNOR_ENGINE Camera
{
public:
	/// @brief Position
	Vector3 position = { 0, 0, 6.f };

	/// @brief Front vector, where the camera is looking
	Vector3 front = -Vector3::UnitZ();
	/// @brief Up vector, where upwards is
	Vector3 up = Vector3::UnitY();
	/// @brief Right vector, where the right is
	Vector3 right = Vector3::UnitX();

	/// @brief Near clipping, threshold where objects too close to the camera won't be rendered
	float_t near = 0.1f;
	/// @brief Far clipping, threshold where objects too far to the camera won't be rendered
	float_t far = 1000.f;
	/// @brief Field of view
	float_t fov = 90.f;

	/// @brief Orthographic left/right
	Vector2 leftRight = { -10.f, 10.f };
	/// @brief Orthographic bottom/top
	Vector2 bottomtop = { -10.f, 10.f };

	/// @brief Whether the camera is orthographic
	bool_t isOrthographic = false;

	Camera() = default;

	~Camera() = default;

	DEFAULT_COPY_MOVE_OPERATIONS(Camera);

	/// @brief Computes the projection @ref Matrix of the camera
	/// @param screenSize Screen size
	/// @param matrix Result matrix
	void GetProjection(Vector2i screenSize, Matrix* matrix) const;

	/// @brief Computes the view @ref Matrix of the camera
	/// @param matrix Result matrix
	void GetView(Matrix* matrix) const;

	/// @brief Computes the projection * view @ref Matrix of the camera
	/// @param screenSize Screen size
	/// @param matrix Result matrix
	void GetVp(Vector2i screenSize, Matrix* matrix) const;

	/// @brief Makes the camera look at a specific position
	/// @param at Where
	void LookAt(const Vector3& at);
	
	/// @brief Makes the camera look at a specific position
	/// @param at Where
	/// @param upVector custom UpVector
	void LookAt(const Vector3& at,const Vector3& upVector);

	/// @brief Projects a 3D position onto the 2D screen coords space
	/// @param vertex 3D position
	/// @param screenSize Screen size
	/// @param model Model @ref Matrix
	/// @return Result position
	[[nodiscard]]
	Vector2i ProjectOn(const Vector3& vertex, Vector2i screenSize, const Matrix& model) const;
	
};

END_XNOR_CORE
