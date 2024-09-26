#pragma once

#include <format>
#include <sstream>

#include <Maths/matrix.hpp>
#include <Maths/quaternion.hpp>
#include <Maths/vector3.hpp>

#include "core.hpp"
#include "reflection/reflection.hpp"

/// @file transform.hpp
/// @brief Defines the XnorCore::Transform class.

BEGIN_XNOR_CORE

class SceneGraph;

/// @brief Represents a 3D transformation, containing Position, Rotation and Scaling
class XNOR_ENGINE Transform final
{
	REFLECTABLE_IMPL(Transform)

public:
	/// @brief World transformation matrix of the transform
	Matrix worldMatrix = Matrix::Identity();
	
	/// @brief Returns the position of this Transform.
	[[nodiscard]]
	const Vector3& GetPosition() const;
	
	/// @brief Sets the position of this Transform.
	void SetPosition(const Vector3& newPosition);
	
	/// @brief Sets the X position of this Transform.
	void SetPositionX(float_t newPositionX);
	
	/// @brief Sets the Y position of this Transform.
	void SetPositionY(float_t newPositionY);
	
	/// @brief Sets the Z position of this Transform.
	void SetPositionZ(float_t newPositionZ);

	/// @brief Returns the rotation of this Transform in euler angles.
	[[nodiscard]]
	const Vector3& GetRotationEulerAngle() const;

	/// @brief Sets the rotation of this Transform in euler angles.
	void SetRotationEulerAngle(const Vector3& newRotationEulerAngle);

	/// @brief Sets the X rotation of this Transform in euler angles.
	void SetRotationEulerAngleX(float_t newRotationEulerAngleX);

	/// @brief Sets the Y rotation of this Transform in euler angles.
	void SetRotationEulerAngleY(float_t newRotationEulerAngleY);

	/// @brief Sets the Z rotation of this Transform in euler angles.
	void SetRotationEulerAngleZ(float_t newRotationEulerAngleZ);

	/// @brief Returns the rotation of this Transform.
	[[nodiscard]]
	const Quaternion& GetRotation() const;

	/// @brief Sets the rotation of this Transform.
	void SetRotation(const Quaternion& newRotation);

	/// @brief Sets the X rotation of this Transform.
	void SetRotationX(float_t newRotationX);

	/// @brief Sets the Y rotation of this Transform.
	void SetRotationY(float_t newRotationY);

	/// @brief Sets the Z rotation of this Transform.
	void SetRotationZ(float_t newRotationZ);

	/// @brief Sets the W rotation of this Transform.
	void SetRotationW(float_t newRotationW);

	/// @brief Returns the scale of this Transform.
	[[nodiscard]]
	const Vector3& GetScale() const;

	/// @brief Sets the scale of this Transform.
	void SetScale(const Vector3& newScale);

	/// @brief Sets the X scale of this Transform.
	void SetScaleX(float_t newScaleX);

	/// @brief Sets the Y scale of this Transform.
	void SetScaleY(float_t newScaleY);

	/// @brief Sets the Z scale of this Transform.
	void SetScaleZ(float_t newScaleZ);

	/// @brief Returns whether at least one of this Transform's field was changed last frame.
	bool_t GetChanged() const;
	
	Vector3 GetRight() const;

	Vector3 GetUp() const;

	Vector3 GetForward() const;
	
private:
	/// @brief Position
	Vector3 m_Position;

	/// @brief Euler rotation
	Vector3 m_EulerRotation;

	/// @brief Scaling
	Vector3 m_Scale = Vector3(1.f);

	/// @brief Quaternion associated with the euler rotation
	Quaternion m_Rotation = Quaternion::Identity();
	
	/// @brief Whether the transform changed and needs to be updated
	bool_t m_Changed = true;

	// SceneGraph is a friend to be able to access the m_Changed private field if the transform changed between 2 frames
	friend class SceneGraph;
};

END_XNOR_CORE

/// @private
REFL_AUTO(type(XnorCore::Transform),
	field(m_Position, XnorCore::Reflection::NotifyChange(&XnorCore::Transform::m_Changed)),
	field(m_EulerRotation, XnorCore::Reflection::NotifyChange(&XnorCore::Transform::m_Changed), XnorCore::Reflection::AsEulerAngles()),
	field(m_Scale, XnorCore::Reflection::NotifyChange(&XnorCore::Transform::m_Changed))
);

#ifndef SWIG
/// @brief @c std::formatter template specialization for the XnorCore::Transform type.
template <>
struct std::formatter<XnorCore::Transform>
{
    /// @brief Parses the input formatting options.
    ///
    ///	The options are specified in 2 parts, the XnorCore::Transform and the @c float options.
    /// The available XnorCore::Transform options are:
    /// - `m`: Uses line breaks for easier readability
    /// - `M`: Prints the world matrix instead of the XnorCore::Transform components
    /// - `q`: Uses a Quaternion for the rotation instead of an euler angles Vector3. Incompatible with option `M`
    /// - `d`: Uses degrees for rotation instead of radians. Incompatible with option `M` or `q`
	template<class ParseContext>
	constexpr typename ParseContext::iterator parse(ParseContext& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;

		if (*it == 'm')
		{
			m_Multiline = true;
			++it;
		}

		if (*it == 'M')
		{
			m_WorldMatrix = true;
			++it;
		}

		if (*it == 'q')
		{
			if (m_WorldMatrix)
				throw std::format_error("Invalid format args for XnorCore::Transform: cannot use option 'q' with option 'M'");
            
			m_QuaternionRotation = true;
			++it;
		}

		if (*it == 'd')
		{
			if (m_WorldMatrix || m_QuaternionRotation)
				throw std::format_error("Invalid format args for XnorCore::Transform: cannot use option 'd' with option 'M' or 'q'");
            
			m_DegreeRotation = true;
			++it;
		}

		while (*it != '}' && it != ctx.end())
			m_Format += *it++;

		return it;
	}

	/// @brief Formats a string using the given instance of XnorCore::Transform, according to the given options in the parse function.
	template<class FormatContext>
	typename FormatContext::iterator format(const XnorCore::Transform& transform, FormatContext& ctx) const
	{
		std::ostringstream out;

		if (m_WorldMatrix)
		{
			std::string beg = "{:";
			if (m_Multiline)
				beg += 'm';
			out << std::vformat(beg + m_Format + '}', std::make_format_args(transform.worldMatrix));
		}
		else
		{
			std::format_args args;
			if (m_QuaternionRotation)
				args = std::make_format_args(transform.GetPosition(), transform.GetRotation(), transform.GetScale());
			else
				args = std::make_format_args(transform.GetPosition(), transform.GetRotationEulerAngle() * (m_DegreeRotation ? Calc::Rad2Deg : 1.f), transform.GetScale());
        
			const char separator = m_Multiline ? '\n' : ';';

			out << std::vformat(
				"{{ {:" + m_Format + "} }} " + separator + "{{ {:" + m_Format + "} }} " + separator + " {{ {:" + m_Format + "} }}",
				args
			);
		}
        
		return std::ranges::copy(std::move(out).str(), ctx.out()).out;
	}

private:
	std::string m_Format;
	bool m_Multiline = false;
	bool m_QuaternionRotation = false;
	bool m_DegreeRotation = false;
	bool m_WorldMatrix = false;
};
#endif
