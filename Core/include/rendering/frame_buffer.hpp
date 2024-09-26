#pragma once

#include "core.hpp"
#include "window.hpp"
#include "resource/texture.hpp"

/// @file frame_buffer.hpp
/// @brief Defines the XnorCore::FrameBuffer class

BEGIN_XNOR_CORE

class RenderPass;

/// @brief Encapsulates a framebuffer object
class Framebuffer
{
public:
	XNOR_ENGINE Framebuffer();
	XNOR_ENGINE ~Framebuffer();

	DEFAULT_COPY_MOVE_OPERATIONS(Framebuffer)

	/// @brief Creates the framebuffer with a render pass and texture attachments
	/// @param renderPass Render pass
	/// @param attachments Texture attachments
	XNOR_ENGINE void AttachTextures(const RenderPass& renderPass, const std::vector<const Texture*>& attachments) const;

	/// @brief Attach a single texture to the framebuffer
	/// @param texture Texture
	/// @param attachment Attachment
	/// @param level Level
	XNOR_ENGINE void AttachTexture(const Texture& texture, Attachment::Attachment attachment, uint32_t level = 0) const;
	
	/// @brief Attach a texture layer to the framebuffer
	/// @param texture Cubemap
	/// @param attachment Attachment
	/// @param level Level
	/// @param layer Layer
	XNOR_ENGINE void AttachTextureLayer(const Texture& texture, Attachment::Attachment attachment, uint32_t level, uint32_t layer) const;

	/// @brief Reads a pixel from an attachment of the framebuffer
	/// @param attachmentIndex Attachment index
	/// @param position Position in the texture
	/// @param textureFormat Texture attachment format
	/// @param dataType Data type
	/// @param output Pixel output
	XNOR_ENGINE void GetPixelFromAttachment(uint32_t attachmentIndex, Vector2i position, TextureFormat::TextureFormat textureFormat, DataType::DataType dataType, void* output) const;
	
	/// @brief Gets the id of the framebuffer
	/// @return Framebuffer id
	XNOR_ENGINE uint32_t GetId() const;
	
private:
	uint32_t m_Id = 0;
};

END_XNOR_CORE
