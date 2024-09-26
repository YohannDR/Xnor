#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "material.hpp"
#include "rhi_typedef.hpp"
#include "vertex.hpp"
#include "buffer/uniform_buffer.hpp"
#include "render_systems/skybox_parser.hpp"

/// @file rhi.hpp
/// Defines the XnorCore::Rhi static class

BEGIN_XNOR_CORE

/// @brief Stands for Render Hardware Interface, provides a set of functions that interface between the application and the rendering API
class Rhi
{
	STATIC_CLASS(Rhi)
	
public:
	/// @brief Skybox parser
	XNOR_ENGINE static inline SkyBoxParser skyBoxParser;

	/// @brief Sets the polygon mode
	/// @param face Polygon face
	/// @param mode Polygon mode
	XNOR_ENGINE static void SetPolygonMode(PolygonFace::PolygonFace face, PolygonMode::PolygonMode mode);
	
	/// @brief Sets the viewport mode
	/// @param screenSize Screen size
	/// @param screenOffset Screen Offset
	XNOR_ENGINE static void SetViewport(Vector2i screenOffset, Vector2i screenSize);
	
	/// @brief Begins a render pass internally
	/// beginInfo Render pass begin info
	XNOR_ENGINE static void BeginRenderPassInternal(const RenderPassBeginInfo& beginInfo);

	/// @brief Ends a render pass
	XNOR_ENGINE static void EndRenderPass();

	/// @brief Creates a model
	/// @param vertices Model vertices
	/// @param indices Model indices
	/// @return Model id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateModel(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	/// @brief Destroys a model
	/// @param modelId Model id
	/// @return Whether the model could be destroyed
	XNOR_ENGINE static bool_t DestroyModel(uint32_t modelId);

	/// @brief Draws a model
	/// @param drawMode Draw mode
	/// @param modelId Model id
	XNOR_ENGINE static void DrawModel(ENUM_VALUE(DrawMode) drawMode, uint32_t modelId); 
	
	XNOR_ENGINE static void DrawArray(DrawMode::DrawMode drawMode,uint32_t first, uint32_t count);
	
	/// @brief Destroy a shader program
	/// @param shaderId Shader id
	XNOR_ENGINE static void DestroyProgram(uint32_t shaderId);

	XNOR_ENGINE static uint32_t ReloadProgram(uint32_t oldShaderId, const std::vector<ShaderCode>& shaderCodes);

	/// @brief Checks for compilation errors for shaders
	/// @param shaderId Shader id
	/// @param type Type
	XNOR_ENGINE static void CheckCompilationError(uint32_t shaderId, const std::string& type);

	/// @brief Creates a shader program using multiple shaders
	/// @param shaderCodes Shader codes
	/// @param shaderCreateInfo Create info
	/// @return Shader id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateShaders(const std::vector<ShaderCode>& shaderCodes, const ShaderCreateInfo& shaderCreateInfo);

	/// @brief Binds a shader for use
	/// @param shaderId Shader id
	XNOR_ENGINE static void UseShader(uint32_t shaderId);
	
	/// @brief Unbinds the current shader
	XNOR_ENGINE static void UnuseShader();

	/// @brief Sets a uniform variable in a shader
	/// @param uniformType Uniform type
	/// @param data Pointer to data
	/// @param shaderId Shader id
	/// @param uniformKey Uniform variable name
	XNOR_ENGINE static void SetUniform(UniformType::UniformType uniformType, const void* data, uint32_t shaderId, const char_t* uniformKey);

	/// @brief Creates a texture
	/// @brief textureCreateInfo Texture creation info
	/// @returns Texture id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateTexture(const TextureCreateInfo& textureCreateInfo);
	
	/// @brief Destroys a texture
	/// @param textureId Texture id
	XNOR_ENGINE static void DestroyTexture(uint32_t textureId);

	/// @brief Binds a texture
	/// @param unit Texture unit
	/// @param textureId Texture id
	XNOR_ENGINE static void BindTexture(uint32_t unit, uint32_t textureId);

	/// @brief Creates a framebuffer
	/// @returns Frame buffer id
	[[nodiscard]]
	XNOR_ENGINE static uint32_t CreateFrameBuffer();
	
	/// @brief Create a framebuffer
	/// @param renderPass Associated RenderPass
	/// @param frameBuffer Framebuffer
	/// @param attachments Texture attachments
	XNOR_ENGINE static void AttachsTextureToFrameBuffer(const RenderPass& renderPass, const Framebuffer& frameBuffer, const std::vector<const Texture*>& attachments);

	/// @brief Destroys a framebuffer
	/// @param frameBufferId Framebuffer id
	XNOR_ENGINE static void DestroyFrameBuffer(uint32_t frameBufferId);

	/// @brief Performs a blit framebuffer operation, which copies a region of pixels from one framebuffer to another
	/// @param readBuffer Source framebuffer id
	/// @param targetBuffer Target framebuffer id
	/// @param srcTopLeft Top left position of the source region
	/// @param srcBottomRight Bottom right position of the source region
	/// @param targetTopLeft Top left position of the target region
	/// @param targetBottomRight Top left position of the target region
	/// @param bufferFlag Target attachment
	/// @param textureFiltering Interpolation to be applied
	XNOR_ENGINE static void BlitFrameBuffer(uint32_t readBuffer, uint32_t targetBuffer, Vector2i srcTopLeft, Vector2i srcBottomRight,
		Vector2i targetTopLeft, Vector2i targetBottomRight, BufferFlag::BufferFlag bufferFlag, TextureFiltering::TextureFiltering textureFiltering);

	/// @brief Binds a framebuffer
	/// @param frameBufferId Framebuffer id
	XNOR_ENGINE static void BindFrameBuffer(uint32_t frameBufferId);

	/// @brief Unbinds a framebuffer
	XNOR_ENGINE static void UnbindFrameBuffer();

	/// @brief Attach a texture to a frame buffer layer
	/// @param bufferId Frame buffer id
	/// @param attachment Attachment
	/// @param textureId Texture id
	/// @param level Texture level
	/// @param layer Frame buffer layer
	XNOR_ENGINE static void AttachTextureToFrameBufferLayer(uint32_t bufferId, Attachment::Attachment attachment, uint32_t textureId, uint32_t level, uint32_t layer);

	/// @brief Attach a texture to a frame buffer
	/// @param bufferId Frame buffer id
	/// @param attachment Attachment
	/// @param textureId Texture id
	/// @param level Texture level
	XNOR_ENGINE static void AttachTextureToFrameBuffer(uint32_t bufferId, Attachment::Attachment attachment, uint32_t textureId, uint32_t level);

	/// @brief Attach a texture from a cubemap to a frame buffer
	/// @param bufferId Frame buffer id
	/// @param attachment Attachment
	/// @param cubeMapFace Cubemap face
	/// @param textureId Texture id
	/// @param level Texture level
	XNOR_ENGINE static void AttachTextureToFrameBuffer(uint32_t bufferId, Attachment::Attachment attachment, CubeMapFace cubeMapFace, uint32_t textureId, uint32_t level);

	/// @brief Sets the target framebuffer for drawing
	/// @param frameBufferid Frame buffer id
	/// @param value Value
	XNOR_ENGINE static void SetFrameBufferDraw(uint32_t frameBufferid, uint32_t value);

	/// @brief Reads a single pixel of an attachment
	/// @param attachmentIndex Attachment index
	/// @param position Pixel position
	/// @param textureFormat Texture format
	/// @param dataType Data type
	/// @param output Output pointer
	XNOR_ENGINE static void GetPixelFromAttachement(uint32_t attachmentIndex, Vector2i position, TextureFormat::TextureFormat textureFormat, DataType::DataType dataType, void* output);

	/// @brief Swaps the front and back buffer
	XNOR_ENGINE static void SwapBuffers();

	/// @brief Initializes the Rhi
	XNOR_ENGINE static void Initialize();

	/// @brief Shutdowns the Rhi
	XNOR_ENGINE static void Shutdown();

	/// @brief Prepares the Render resource
	XNOR_ENGINE static void PrepareRendering();

	/// @brief Sets the clear color
	/// @param color Clear color
	XNOR_ENGINE static void SetClearColor(const Vector4& color);

	/// @brief Clear the corresponding Buffers
	/// @param bufferFlag BufferFlag
	XNOR_ENGINE static void ClearBuffer(BufferFlag::BufferFlag bufferFlag);
	
	/// @brief Updates the model UniformBuffer
	/// @param modelUniformData Data
	XNOR_ENGINE static void UpdateModelUniform(const ModelUniformData& modelUniformData);

	/// @brief Updates the camera UniformBuffer
	/// @param cameraUniformData Data
	XNOR_ENGINE static void UpdateCameraUniform(const CameraUniformData& cameraUniformData);

	/// @brief Updates the animation UniformBuffer
	/// @param skinnedMeshGPUData Data
	XNOR_ENGINE static void UpdateAnimationUniform(const SkinnedMeshGpuData& skinnedMeshGPUData);

	/// @brief Updates the light UniformBuffer
	/// @param lightData Data
	XNOR_ENGINE static void UpdateLight(const GpuLightData& lightData);
	
	/// @brief Binds a Material
	/// @param material Material
	XNOR_ENGINE static void BindMaterial(const Material& material);

	/// @brief Gets the texture format based on the number of color channels
	/// @param channels Color channels
	/// @return Texture format
	XNOR_ENGINE static TextureFormat::TextureFormat GetTextureFormatFromChannels(uint32_t channels);

	/// @brief Enables/disables the depth test
	/// @param value Value
	XNOR_ENGINE static void DepthTest(bool_t value);

	/// @brief Gets the cubemap view matrices
	XNOR_ENGINE static void GetCubeMapViewMatrices(std::array<Matrix, 6>* viewsMatricies);

	/// @brief Dispatch a compute shader
	/// @param numberOfGroupX Number of groups on the X axis
	/// @param numberOfGroupY Number of groups on the Y axis
	/// @param numberOfGroupZ Number of groups on the Z axis
	XNOR_ENGINE static void DispatchCompute(uint32_t numberOfGroupX, uint32_t numberOfGroupY, uint32_t numberOfGroupZ);

	/// @brief Sets the GPU memory barrier
	/// @param memoryBarrier Memory barrier
	XNOR_ENGINE static void SetGpuMemoryBarrier(GpuMemoryBarrier memoryBarrier);

	/// @brief Binds an image texture
	/// @param unit Image unit
	/// @param texture Texture id
	/// @param level Level
	/// @param layered Whether it's layered
	/// @param layer Layer
	/// @param imageAcess Image access
	/// @param textureInternalFormat Texture internal format
	XNOR_ENGINE static void BindImageTexture(uint32_t unit, uint32_t texture, uint32_t level, bool_t layered, uint32_t layer, ImageAccess imageAcess, TextureInternalFormat::TextureInternalFormat textureInternalFormat);
	
	// Enum to OpenglEnum
	XNOR_ENGINE static uint32_t GetOpengDepthEnum(DepthFunction::DepthFunction depthFunction);
	
	XNOR_ENGINE static uint32_t BlendEquationToOpengl(BlendEquation::BlendEquation blendEquation);
	
	XNOR_ENGINE static uint32_t GetOpenglShaderType(ShaderType::ShaderType shaderType);
	
	XNOR_ENGINE static std::string GetShaderTypeToString(ShaderType::ShaderType shaderType);
	
	XNOR_ENGINE static uint32_t GetOpenglTextureType(TextureType::TextureType textureType);
	
	XNOR_ENGINE static uint32_t GetOpenglInternalFormat(TextureInternalFormat::TextureInternalFormat textureFormat);
	
	XNOR_ENGINE static uint32_t GetOpenGlTextureFormat(TextureFormat::TextureFormat textureFormat);
	
	XNOR_ENGINE static uint32_t GetOpenglTextureWrapper(TextureWrapping::TextureWrapping textureWrapping);
	
	XNOR_ENGINE static uint32_t GetOpenglTextureFilter(TextureFiltering::TextureFiltering textureFiltering);
	
	XNOR_ENGINE static uint32_t GetBlendValueOpengl(BlendValue::BlendValue blendFunction);
	
	XNOR_ENGINE static uint32_t GetOpenglBufferBit(BufferFlag::BufferFlag flag);
	
	XNOR_ENGINE static uint32_t AttachementToOpenglAttachement(Attachment::Attachment attachment);
	
	XNOR_ENGINE static uint32_t CubeMapFacesToOpengl(CubeMapFace cubeMapFace);
	
	XNOR_ENGINE static uint32_t MemoryBarrierToOpengl(GpuMemoryBarrier memoryBarrier);
	
	XNOR_ENGINE static uint32_t GetImageAccessOpengl(ImageAccess imageAcess);

	XNOR_ENGINE static uint32_t FrontFaceToOpenglFrontFace(FrontFace::FrontFace frontFace);
	
	XNOR_ENGINE static uint32_t CullFaceToOpenglCullFace(CullFace::CullFace cullFace);

	XNOR_ENGINE static uint32_t GetOpenglDataType(DataType::DataType dataType);

	XNOR_ENGINE static uint32_t DrawModeToOpengl(DrawMode::DrawMode drawMode);

	XNOR_ENGINE static void SetPixelStore(DataAlignment alignement, int32_t value);

	XNOR_ENGINE static uint32_t BufferUsageToOpenglUsage(BufferUsage usage);

private:
	struct ModelInternal
	{
		uint32_t vao = 0;
		uint32_t vbo = 0;
		uint32_t ebo = 0;
		uint32_t nbrOfVertex = 0;
		uint32_t nbrOfIndicies = 0;
	};
	
	struct ShaderInternal
	{
		DepthFunction::DepthFunction depthFunction{};
		BlendFunction blendFunction;
		ShaderProgramCullInfo cullInfo;
		std::unordered_map<std::string, GpuUniform> uniformMap;
	};

	XNOR_ENGINE static inline UniformBuffer* m_CameraUniform = nullptr;
	XNOR_ENGINE static inline UniformBuffer* m_ModelUniform = nullptr;
	XNOR_ENGINE static inline UniformBuffer* m_LightUniform = nullptr;
	XNOR_ENGINE static inline UniformBuffer* m_MaterialUniform = nullptr;
	XNOR_ENGINE static inline UniformBuffer* m_AnimationBuffer = nullptr;
	
	XNOR_ENGINE static inline bool_t m_Blending = false;
	
	XNOR_ENGINE static inline bool_t m_Cullface = false;
	
	XNOR_ENGINE static inline bool_t m_Depth = true;

	static constexpr int32_t NullUniformLocation = -1;
	
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ShaderInternal> m_ShaderMap;
	
	XNOR_ENGINE static inline std::unordered_map<uint32_t, ModelInternal> m_ModelMap;

	XNOR_ENGINE static void LogComputeShaderInfo();
	
	XNOR_ENGINE static void IsShaderValid(uint32_t shaderId);
	
	XNOR_ENGINE static GpuUniform& GetUniformInMap(uint32_t shaderId, const char_t* uniformKey, UniformType::UniformType type);
	
	// Texture 
	XNOR_ENGINE static uint32_t CreateTextureId(TextureType::TextureType textureType);
	
	/// @brief Set the depth function to the graphic API
	/// @param depthFunction The current depthFunction to set to the api
	XNOR_ENGINE static void ComputeDepthFunction(DepthFunction::DepthFunction depthFunction);

	XNOR_ENGINE static void OpenglDebugCallBack(
		uint32_t source,
		uint32_t type,
		uint32_t id,
		uint32_t severity,
		int32_t length,
		const char_t* message,
		const void* userParam
	);
	
	XNOR_ENGINE static void ComputeTextureFiltering(TextureType::TextureType textureType, uint32_t textureId,  const TextureCreateInfo& textureCreateInfo);
	
	XNOR_ENGINE static void ComputeTextureWrapping(TextureType::TextureType textureType, uint32_t textureId, const TextureCreateInfo& textureCreateInfo);
	
	XNOR_ENGINE static void AllocTexture(TextureType::TextureType textureType, uint32_t textureId, const TextureCreateInfo& textureCreateInfo);

	XNOR_ENGINE static bool IsDataValid(const std::vector<void*>& data,size_t wantedSize = 1);
};

END_XNOR_CORE
