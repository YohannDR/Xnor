#pragma once

#include "core.hpp"

#include <array>
#include <Maths/vector2.hpp>
#include <Maths/vector3.hpp>
#include <Maths/vector4.hpp>
#include <Maths/matrix.hpp>
#include <Maths/matrix3.hpp.>

#include "texture.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"

/// @file compute_shader.hpp
/// @brief Defines the XnorCore::MeshCollider class

BEGIN_XNOR_CORE

/// @brief Encapsulates a compute shader
class ComputeShader : public Resource
{
public:
    /// @brief Allowed extensions for compute shaders
    XNOR_ENGINE static constexpr std::array<const char_t*, 2> ComputeFileExtensions
    {
        ".comp",
        ".compute"
    };

    using Resource::Resource;

    using Resource::Load;

	XNOR_ENGINE ComputeShader() = default;
	XNOR_ENGINE ~ComputeShader() override = default;

    DEFAULT_COPY_MOVE_OPERATIONS(ComputeShader)

    /// @brief Loads a single shader file
	/// @param shader File
	/// @return Whether the load succeeded
	XNOR_ENGINE bool_t Load(const Pointer<File>& shader) override;

	/// @brief Loads raw shader code
	/// @param buffer Raw data
	/// @param length Raw data length
	XNOR_ENGINE bool_t Load(const char_t* buffer, int64_t length);

	/// @brief Creates the shader in the @ref Rhi
	XNOR_ENGINE void CreateInInterface() override;

	/// @brief Destroys the shader in the @ref Rhi
	XNOR_ENGINE void DestroyInInterface() override;

	/// @brief Recompiles the shader
	XNOR_ENGINE void Recompile();

	/// @brief Unloads the shader
	XNOR_ENGINE void Unload() override;

	/// @brief Sets an int (signed, 32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetInt(const std::string& keyName, int32_t value) const;

	/// @brief Sets an bool (signed, 32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetBool(const std::string& keyName, bool_t value) const;

	/// @brief Sets an float (32 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetFloat(const std::string& keyName, float_t value) const;

	/// @brief Sets a @ref Vector2 (2 float, 64 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetVec2(const std::string& keyName, const Vector2& value) const;
	
	/// @brief Sets a @ref Vector3 (3 float, 96 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetVec3(const std::string& keyName, const Vector3& value) const;
	
	/// @brief Sets a @ref Vector4 (4 float, 128 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetVec4(const std::string& keyName, const Vector4& value) const;

	/// @brief Sets a @ref Matrix (16 float, 512 bits) variable in a shader
	/// @param keyName Variable name
	/// @param value Value
	XNOR_ENGINE void SetMat4(const std::string& keyName, const Matrix& value) const;

	/// @brief Gets the internal id of the shader
	/// @return Id
	[[nodiscard]]
	XNOR_ENGINE uint32_t GetId() const;

	/// @brief Binds the shader for use
	XNOR_ENGINE void Use() const;

	/// @brief Unbinds the shader
	XNOR_ENGINE void Unuse() const;

	/// @brief Dispatch the compute shader
	/// @param numberOfGroupX Number of groups on the X axis
	/// @param numberOfGroupY Number of groups on the Y axis
	/// @param numberOfGroupZ Number of groups on the Z axis
	XNOR_ENGINE void DispatchCompute(uint32_t numberOfGroupX, uint32_t numberOfGroupY, uint32_t numberOfGroupZ) const;

	/// @brief Binds an image texture
	/// @param unit Image unit
	/// @param texture Texture id
	/// @param level Level
	/// @param layered Whether it's layered
	/// @param layer Layer
	/// @param imageAcess Image access
	XNOR_ENGINE void BindImage(uint32_t unit, const Texture& texture, uint32_t level, bool_t layered, uint32_t layer, ImageAccess imageAcess) const;

	/// @brief Sets the memory barrier
	/// @param memoryBarrier Memory barrier
	XNOR_ENGINE void SetMemoryBarrier(GpuMemoryBarrier memoryBarrier) const;

private:
	uint32_t m_Id = 0;
	
	ShaderCode m_ShaderCode;
	Pointer<File> m_File;
};

END_XNOR_CORE
