#pragma once

#include "core.hpp"
#include "Maths/vector2.hpp"
#include "Maths/vector3.hpp"

/// @file vertex.hpp
/// @brief Defines the XnorCore::Vertex class.

BEGIN_XNOR_CORE

/// @brief Vertex containing a position, a normal, texture coordinates, a tangent and a bi-tangent.
/// 
/// @see <a href="https://en.wikipedia.org/wiki/Vertex_(computer_graphics)">Vertex Wikipedia page</a>
struct Vertex
{
	static constexpr size_t MaxBoneWeight = 4;
	
	/// @brief Position in 3 dimension.
	Vector3 position;
	/// @brief Normal of the vertex
	Vector3 normal;
	/// @brief Texture coordinates in the range [0, 1].
	Vector2 textureCoord;
	Vector3 tangent;
	Vector3 bitangent;

	float_t boneIndices[MaxBoneWeight] = { -1, -1, -1, -1 };
	float_t boneWeight[MaxBoneWeight] = {};
};

END_XNOR_CORE
