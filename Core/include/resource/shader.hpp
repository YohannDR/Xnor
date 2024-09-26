#pragma once

#include <array>

#include "core.hpp"
#include "file/file.hpp"
#include "Maths/math.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource.hpp"
#include "utils/pointer.hpp"
#include "reflection/reflection.hpp"

/// @file shader.hpp
/// @brief Defines the XnorCore::Shader class

BEGIN_XNOR_CORE

/// @brief Encapsulates a GPU shader
class Shader final : public Resource
{
	ShaderPipeline::ShaderPipeline ShaderTypeToShaderPipeline(ShaderType::ShaderType shaderType);

	ShaderType::ShaderType ShaderPipelineToShaderType(ShaderPipeline::ShaderPipeline shaderPipeline);

public:
	/// @brief Allowed extensions for vertex shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> VertexFileExtensions
	{
		".vert",
		".vertex",
		".vs"
	};

	/// @brief Allowed extensions for fragment shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FragmentFileExtensions
	{
		".frag",
		".fragment",
		".fs"
	};

	/// @brief Allowed extensions for geometry shaders
	XNOR_ENGINE static inline constexpr std::array<const char_t*, 2> GeometryFileExtensions
	{
		".geom",
		".geometry"
	};
	
	/// @brief Gets the shader type via a file extension
	/// @param extension File extension
	/// @return @ref ShaderType type
	/// @throw std::invalid_argument If the extension isn't valid
	[[nodiscard]]
	XNOR_ENGINE static ShaderPipeline::ShaderPipeline FileExtensionToType(const std::string& extension);

	using Resource::Resource;

	using Resource::Load;

	/// @brief Loads a single shader file
	/// @param shader File
	/// @return Whether the load succeeded
	XNOR_ENGINE bool_t Load(const Pointer<File>& shader) override;

	/// @brief Loads raw shader code
	/// @param buffer Raw data
	/// @param length Raw data length
	/// @param type Shader type
	XNOR_ENGINE bool_t Load(const char_t* buffer, int64_t length, ShaderPipeline::ShaderPipeline type);

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

	/// @brief Sets a specialized depth function for the shader, should only be called it before @ref CreateInInterface
	/// @param depthFunction Depth function
	XNOR_ENGINE void SetDepthFunction(DepthFunction::DepthFunction depthFunction);
	
	/// @brief Sets a specialized blend function for the shader, should only be called it before @ref CreateInInterface
	/// @param blendFunction Blend function
	XNOR_ENGINE void SetBlendFunction(const BlendFunction& blendFunction);

	/// @brief Sets the face culling info of the shader
	/// @param shaderProgramCullInfo Culling info
	XNOR_ENGINE void SetFaceCullingInfo(const ShaderProgramCullInfo& shaderProgramCullInfo);

private:
	uint32_t m_Id = 0;
	
	DepthFunction::DepthFunction m_DepthFunction = DepthFunction::Less;
	ShaderProgramCullInfo m_ShaderProgramCullInfo;
	BlendFunction m_BlendFunction;
	
	std::array<Pointer<File>, ShaderPipeline::Count> m_Files;
	std::array<ShaderCode, ShaderPipeline::Count> m_Code;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Shader, bases<XnorCore::Resource>))
