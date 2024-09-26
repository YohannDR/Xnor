#pragma once

#include <vector>

#include <Maths/matrix.hpp>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "rendering/vertex.hpp"


/// @file rhi_typedef.hpp
/// @brief Defines various types and enumerations needed by XnorCore::Rhi.

BEGIN_XNOR_CORE

class Framebuffer;

/// @brief Maximum amount of spot lights that can exists in a same scene
static constexpr uint32_t MaxSpotLights = 50;
/// @brief Maximum amount of point lights that can exists in a same scene
static constexpr uint32_t MaxPointLights = 50;
/// @brief Maximum amount of directional lights that can exists in a same scene
static constexpr uint32_t MaxDirectionalLights = 1;

static constexpr uint32_t MaxBones = 100;

static constexpr size_t DirectionalCascadeLevelAllocation = 12;
static constexpr size_t DirectionalCascadeLevel = 4;


/// @brief Polygon rasterization mode
/// @see <a href="https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml">OpenGL specification</a>
BEGIN_ENUM(PolygonMode)
{
	Point = 0,
	Line,
	Fill,
}
END_ENUM

/// @brief Cull face mode
BEGIN_ENUM(CullFace)
{
	/// @brief No culling
	None,
	/// @brief Cull front faces
	Front,
	/// @brief Cull back faces
	Back,
	/// @brief Cull both front and back faces
	FrontAndBack
}
END_ENUM

/// @brief Front-face mode
BEGIN_ENUM(FrontFace)
{
	/// @brief Clock-wise
	CW,
	/// @brief Counter clock-wise
	CCW
}
END_ENUM

/// @brief Shader program cull info
struct ShaderProgramCullInfo
{
	/// @brief Whether cull face is enabled
	bool_t enableCullFace = false;
	/// @brief Cull face mode
	CullFace::CullFace cullFace = CullFace::Front;
	/// @brief Front-face mode
	FrontFace::FrontFace frontFace = FrontFace::CCW;
};

/// @brief Polygon draw mode
/// @see <a href="https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml">OpenGL specification</a>
BEGIN_ENUM(DrawMode)
{
	Point = 0,
	Line,
	LineLoop,
	LineStrip,
	Triangles,
	TrianglesStrip,
	TrianglesFan,
	LineStripAdjency,
	TrianglesStripAdjency,
	TrianglesAdjency,
}
END_ENUM

/// @brief Polygon face
BEGIN_ENUM(PolygonFace)
{
	FrontLeft = 1024,
	FrontRight,
	BackLeft,
	BackRight,
	Front,
	Back,
	Left,
	Right,
	FrontAndBack 
}
END_ENUM

/// @brief Shader type
BEGIN_ENUM(ShaderType)
{
	Vertex,
	Fragment,
	Geometry,
	Compute,
}
END_ENUM

/// @brief Shader pipeline values
BEGIN_ENUM(ShaderPipeline)
{
	/// @brief Vertex shader
	Vertex,
	/// @brief Fragment shader
	Fragment,
	/// @brief Geometry shader
	Geometry,

	/// @private
	Count
}
END_ENUM

/// @brief Encapsulates shader code information
struct ShaderCode
{
	/// @brief Pointer to raw code
	std::string code;
	/// @brief Raw code length
	int32_t codeLength = 0;
	/// @brief Shader type
	ShaderType::ShaderType type;
};

/// @brief %Texture wrapping, determines how a point will be sampled if it's outside of the texture
BEGIN_ENUM(TextureWrapping)
{
	None,
	/// @brief Repeats the image
	Repeat,
	/// @brief Repeats the image, also mirrors it with each repeat
	MirroredRepeat,
	/// @brief Uses the last valid border pixel of the texture
	ClampToEdge,
	/// @brief Samples a user given pixel
	ClampToBorder,
}
END_ENUM

/// @brief %Texture filtering, determines how the sampling approximation will be done
BEGIN_ENUM(TextureFiltering)
{
	None,
	/// @brief Performs linear interpolation
	Linear,
	/// @brief Selects the nearest texel
	Nearest,

	NearestMipmapNearest,

	LinearMimapNearest,

	NearestMimapLinear,
	
	LinearMimMapLinear
}
END_ENUM

/// @brief %sTexture type
BEGIN_ENUM(TextureType)
{
	Texture1D,
	Texture2D,
	Texture3D,
	Texture1DArray,
	Texture2DArray,
	TextureRectangle,
	TextureCubeMap,
	TextureCubeMapArray,
	TextureBuffer,
	Texture2DMultisample,
	Texture2DMultisampleArray,
}
END_ENUM

/// @brief %Texture internal format
BEGIN_ENUM(TextureInternalFormat)
{
	R8,
	R16,
	Rg8,
	Rg16,
	Rgb8,
	Rgb16,
	Rgba8,
	Rgba16,
	R16F,
	Rg16F,
	Rgb16F,
	Rgba16F,
	R32F,
	R32Uint,
	Srgb,
	R11FG11FB10F,
	Rgba32F,
	DepthComponent16,
	DepthComponent24,
	DepthComponent32,
	DepthComponent32F,
	Depth24Stencil8,
	DepthComponent32FStencil8
}
END_ENUM

/// @brief %Texture data type
BEGIN_ENUM(DataType)
{
	Byte,
	UnsignedByte,
	Short,
	UnsignedShort,
	Int,
	UnsignedInt,
	Float,
	Double
}
END_ENUM

/// @brief %Texture format
BEGIN_ENUM(TextureFormat)
{
	Red,
	RedGreen,
	Rgb,
	Rgba,
	DepthComponent
}
END_ENUM

/// @brief %Framebuffer %attachment
BEGIN_ENUM(Attachment)
{
	/// @brief Color attachment 0
	Color00,
	/// @brief Color attachment 1
	Color01,
	/// @brief Color attachment 2
	Color02,
	/// @brief Color attachment 3
	Color03,
	/// @brief Color attachment 4
	Color04,
	/// @brief Color attachment 5
	Color05,
	/// @brief Color attachment 6
	Color06,
	/// @brief Color attachment 7
	Color07,
	/// @brief Color attachment 8
	Color08,
	/// @brief Color attachment 9
	Color09,
	/// @brief Color attachment 10
	Color10,
	/// @brief Color attachment 11
	Color11,
	/// @brief Color attachment 12
	Color12,
	/// @brief Color attachment 13
	Color13,
	/// @brief Color attachment 14
	Color14,
	/// @brief Color attachment 15
	Color15,
	/// @brief Color attachment 16
	Color16,
	/// @brief Color attachment 17
	Color17,
	/// @brief Color attachment 18
	Color18,
	/// @brief Color attachment 19
	Color19,
	/// @brief Color attachment 20
	Color20,
	/// @brief Depth attachment
	Depth,
	/// @brief Stencil attachment
	Stencil,
	/// @brief Depth and stencil attachment
	DepthAndStencil,
}
END_ENUM

/// @brief Render target info for RenderPass
struct RenderTargetInfo
{
	/// @brief Attachment
	Attachment::Attachment attachment;

	bool_t isDrawingOn = true;
};

/// @brief Texture creation info
struct TextureCreateInfo
{
	TextureType::TextureType textureType = TextureType::Texture2D;
	
	/// @brief Data
	std::vector<void*> datas;
	/// @brief nbr mipmap
	uint32_t mipMaplevel = 1;
	/// @brief nbr of texture level 
	uint32_t depth = 1;
	/// @brief Size
	Vector2i size = Vector2i::Zero();
	/// @brief Filtering
	TextureFiltering::TextureFiltering filtering{};
	/// @brief Wrapping
	TextureWrapping::TextureWrapping wrapping{};
	/// @brief Format
	TextureFormat::TextureFormat format{};
	/// @brief Internal format
	TextureInternalFormat::TextureInternalFormat internalFormat{};
	/// @brief Data type
	DataType::DataType dataType = DataType::UnsignedByte;
	/// @brief Border values 
	float_t borderColor[4] = { 1.f, 1.f, 1.f, 1.f };
};

/// @brief Camera UniformBuffer data
struct CameraUniformData
{
	/// @brief View matrix
	Matrix view = Matrix::Identity();
	/// @brief Projection matrix
	Matrix projection = Matrix::Identity();
	/// @brief View matrix
	Matrix invView = Matrix::Identity();
	/// @brief Projection matrix
	Matrix invProjection = Matrix::Identity();
	
	/// @brief Camera position
	Vector3 cameraPos;

	float_t near;

	float_t far;
};

/// @brief Model UniformBuffer data
struct ModelUniformData
{
	/// @brief Model matrix
	Matrix model = Matrix::Identity();
	/// @brief Model matrix (inverted and transposed)
	Matrix normalInvertMatrix = Matrix::Identity();

	uint64_t meshRenderIndex = 0;
};

/// @brief Uniform type for Shader
BEGIN_ENUM(UniformType)
{
	/// @brief Int
	Int,
	/// @brief Bool
	Bool,
	/// @brief Float
	Float,
	/// @brief Vector3
	Vec2,
	/// @brief Vector3
	Vec3,
	/// @brief Vector4
	Vec4,
	/// @brief Matrix3
	Mat3,
	/// @brief Matrix
	Mat4,
}
END_ENUM

struct GpuUniform
{
	ENUM_VALUE(UniformType) type;
	uint32_t shaderKey;
	union
	{
		int32_t Int;
		int32_t Bool;
		float_t Float;
		Vector2 Vec2;
		Vector3 Vec3;
		Vector4 Vec4;
		Matrix3 Mat3;
		Matrix Mat4;
	} data = {};
};

/// @brief Depth function
BEGIN_ENUM(DepthFunction)
{
	/// @brief Always
	Always,
	/// @brief Never
	Never,
	/// @brief <
	Less,
	/// @brief ==
	Equal,
	/// @brief <=
	LessEqual,
	/// @brief >
	Greater,
	/// @brief !=
	NotEqual,
	/// @brief >=
	GreaterEqual,

	/// @brief !Depth
	Disable
}
END_ENUM

/// @brief Blend value
BEGIN_ENUM(BlendValue)
{
	/// @brief 0
	Zero,
	/// @brief 1
	One,
	/// @brief Src
	SrcColor,
	/// @brief 1 - Src
	OneMinusSrcColor,
	/// @brief Dst
	DstColor,
	/// @brief 1 - Dst
	OneMinusDstColor,
	/// @brief Src.a
	SrcAlpha,
	/// @brief 1 - Src.a
	OneMinusSrcAlpha,
	/// @brief Dst.a
	DstAlpha,
	/// @brief 1 - Dst.a
	OneMinusDstAlpha,
	/// @brief Ct
	ConstantColor,
	/// @brief 1 - Ct
	OneMinusConstantColor,
	/// @brief Ct.a
	ConstantAlpha,
	/// @brief 1 - Ct.a
	OneMinusConstantAlpha,
}
END_ENUM

/// @brief Blend equation
BEGIN_ENUM(BlendEquation)
{
	/// @brief A + B
	Add,
	/// @brief A - B
	Sub,
	/// @brief B - A
	ReverSub,
	/// @brief min(A, B)
	Min,
	/// @brief max(A, B)
	Max,
}
END_ENUM

/// @brief Blend function for Shader
struct BlendFunction
{
	/// @brief Whether blending is enabled
	bool_t isBlending = false;
	/// @brief Source blend constant
	BlendValue::BlendValue sValue = BlendValue::One;
	/// @brief Destination blend constant
	BlendValue::BlendValue dValue = BlendValue::Zero;
	/// @brief Blend equation
	BlendEquation::BlendEquation blendEquation = BlendEquation::Add;
};

/// @brief Shader creation info
struct ShaderCreateInfo
{
	/// @brief Depth function
	DepthFunction::DepthFunction depthFunction{};
	/// @brief Blend function
	BlendFunction blendFunction{};

	/// @brief Shader program cull info
	ShaderProgramCullInfo shaderProgramCullInfo;
};

// Remove Warning From alignas(16)
#pragma warning(push)
#pragma warning(disable : 4324)

/// @brief Point light UniformBuffer data
struct ALIGNAS(16) PointLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Position
	Vector3 position;
	/// @brief Radius
	float_t radius{};
	/// @brief CastShadow
	int32_t isCastingShadow = 0;
};

/// @brief Spot light UniformBuffer data
struct ALIGNAS(16) SpotLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Position
	Vector3 position;
	/// @brief Cut-off
	float_t cutOff{};
	/// @brief Direction
	Vector3 direction;
	/// @brief Outer cut-off
	float_t outerCutOff{};
	
	/// @brief CastShadow
	int32_t isCastingShadow = 0;
};

/// @brief Directional light UniformBuffer data
struct ALIGNAS(16) DirectionalLightData
{
	/// @brief Color
	Vector3 color;
	/// @brief Intensity
	float_t intensity{};
	/// @brief Direction
	Vector3 direction;
	
	/// @brief CastShadow
	int32_t isDirlightCastingShadow = 0;

	int32_t cascadeCount = DirectionalCascadeLevel;

	float_t cascadePlaneDistance[DirectionalCascadeLevel];
};

/// @brief Light UniformBuffer data
struct ALIGNAS(16) GpuLightData
{
	/// @brief Number of active point lights
	uint32_t nbrOfPointLight{};
	/// @brief Number of active spot lights
	uint32_t nbrOfSpotLight{};
	
	/// @brief Point light data
	PointLightData pointLightData[MaxPointLights];
	/// @brief Spot light data
	SpotLightData spotLightData[MaxSpotLights];
	/// @brief Directional light data
	DirectionalLightData directionalData[MaxDirectionalLights];
	
	/// @brief LightSpaceMatrix for shadowMapping
	Matrix spotLightSpaceMatrix[MaxSpotLights];

	/// @brief Light space matrix
	Matrix dirLightSpaceMatrix[DirectionalCascadeLevelAllocation];
	
	/// @brief Number of active directional lights shoul be 0 or 1
	uint32_t nbrOfDirLight{};
};

/// @brief UniformBuffer data for animation
struct ALIGNAS(16) SkinnedMeshGpuData
{
	Matrix boneMatrices[MaxBones];
};

#pragma warning(pop) // 4324
	

/// @brief Material UniformBuffer data
struct MaterialData
{
	/// @brief Albedo color
	Vector3 albedoColor;
	/// @brief Whether it has an albedo map
	int32_t hasAlbedoMap;

	/// @brief Emissive color
	Vector3 emissiveColor;
	/// @brief Emissive parameter
	float_t emissive = 0.f;

	int32_t hasEmissive;
	
	/// @brief Whether it has a metallic map
	int32_t hasMetallicMap;
	/// @brief Metallic parameter
	float_t metallic = 0.f;

	/// @brief Whether it has a roughness map
	int32_t hasRoughnessMap;
	/// @brief Roughness parameter
	float_t roughness = 0.f;

	/// @brief Whether it has an ambient occlusion map
	int32_t hasAmbientOcclusionMap;
	/// @brief Ambient occlusion parameter
	float_t ambientOcclusion = 0.f;

	/// @brief Whether it has a normal map map
	int32_t hasNormalMap;
	/// @brief Emissive parameter
	float_t reflectance = 0.f;
};

/// @brief The type of DefferedDescriptor.
BEGIN_ENUM(DefferedDescriptor)
{
	Normal = 5,
	Albedo,
	MetallicRoughessReflectance,
	AmbiantOcclusion,
	Emissivive,
	Depth,
	
	SkyboxIrradiance = 12,
	SkyboxPrefilterMap = 13,
	SkyboxPrecomputeBrdf = 14,
};
END_ENUM

/// @brief The type of GBuffer.
BEGIN_ENUM(ShadowTextureBinding)
{
	Directional = 15,
	SpotLight = 16,
	PointLightCubemapArrayPixelDistance = 17,
};
END_ENUM

/// @brief Material texture
BEGIN_ENUM(MaterialTextureEnum) : int32_t
{
	Albedo = 0,
	Metallic,
	Roughness,
	Normal,
	AmbiantOcclusion,
	EmissiveMap,
};
END_ENUM

/// @brief Buffer attachment flags.
BEGIN_ENUM(BufferFlag) 
{
	None = 0,
	ColorBit = 1 << 0,  
	DepthBit = 1 << 1,
	StencilBit = 1 << 2  
}
END_ENUM


/// @brief Render pass begin info
struct RenderPassBeginInfo
{
	/// @brief Render pass attached
	Framebuffer* frameBuffer = nullptr;
	/// @brief Render area position
	Vector2i renderAreaOffset{};
	/// @brief Render area size
	Vector2i renderAreaExtent{};

	/// @brief Clear buffer flags
	BufferFlag::BufferFlag clearBufferFlags{};

	/// @brief Clear color
	Vector4 clearColor = Vector4::Zero();
};

/// @brief Cube map face
enum class CubeMapFace
{
	CubeMapPositiveX,
	CubeMapNegativeX,
	CubeMapPositiveY,
	CubeMapNegativeY,
	CubeMapPositiveZ,
	CubeMapNegativeZ,

	Size
};

/// @brief GPU memory barrier
enum GpuMemoryBarrier
{
	VertexAttribArrayBarrierBit = 1 << 0,
	ElementArrayBarrierBit = 1 << 1,
	UniformBarrierBit = 1 << 2,
	TextureFetchBarrierBit = 1 << 3,
	ShaderImageAccessBarrierBit = 1 << 4,
	CommandBarrierBit = 1 << 5,
	PixelBufferBarrierBit = 1 << 6,
	TextureUpdateBarrierBit = 1 << 7,
	BufferUpdateBarrierBit = 1 << 8,
	ClientMappedBufferBarrierBit = 1 << 9,
	FramebufferBarrierBit = 1 << 10,
	TransformFeedbackBarrierBit = 1 << 11,
	AtomicCounterBarrierBit = 1 << 12,
	ShaderStorageBarrierBit = 1 << 13,
	QueryBufferBarrierBit = 1 << 14,
	AllBarrierBits = 1 << 15
};

/// @brief GPU image access
enum class ImageAccess
{
	ReadOnly,
	WriteOnly,
	ReadWrite,
};



struct VertexAttributePointer
{
	uint32_t index = 0;
	size_t size = 0;
	DataType::DataType bufferDatatype = DataType::Byte;
	bool_t normalized = false;
	size_t stride = 0;
	const void* pointer = nullptr;
	
};

struct AttributeDivisor
{
	uint32_t index = 0;
	uint32_t divisor = 0;
};

struct VertexAttributeBinding
{
	uint32_t attribIndex = 0;
	uint32_t bindingIndex = 0;
};

struct VertexAttribFormat
{
	 uint32_t attribIndex = 0;
	 uint32_t size = 0;
	 DataType::DataType type = DataType::Float;
	 bool_t normalized = false;
	 uint32_t relativeOffset = 0;
};

struct VaoDescriptor
{
	const VertexAttributeBinding* vertexAttributeBindings = nullptr;
	size_t vertexAttributeBindingSize = 0;
	
	const VertexAttribFormat* vertexAttribFormats = nullptr;
	size_t vertexAttribFormatsSize = 0;

	uint32_t vboId = 0;
};

enum class DataAlignment
{
	Pack,
	UnPack
};

enum class BufferUsage
{
	StreamDraw,
	StreamRead,
	StreamCopy,
	StaticDraw,
	StaticRead,
	StaticCopy,
	DynamicDraw,
	DrawRead,
	ReadCopy,
};

END_XNOR_CORE
