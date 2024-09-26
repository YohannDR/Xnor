#include "rendering/frame_buffer.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"

using namespace XnorCore;

Framebuffer::Framebuffer()
{
	m_Id = Rhi::CreateFrameBuffer();
}

Framebuffer::~Framebuffer()
{
	Rhi::DestroyFrameBuffer(m_Id);
}

void Framebuffer::AttachTextures(const RenderPass& renderPass, const std::vector<const Texture*>& attachments) const 
{
	Rhi::AttachsTextureToFrameBuffer(renderPass, *this, attachments);
}

void Framebuffer::AttachTexture(const Texture& texture, const Attachment::Attachment attachment, const uint32_t level) const
{
	Rhi::AttachTextureToFrameBuffer(m_Id, attachment, texture.GetId(), level);
}

void Framebuffer::AttachTextureLayer(const Texture& texture, const Attachment::Attachment attachment, const uint32_t level, const uint32_t layer) const
{
	Rhi::AttachTextureToFrameBufferLayer(m_Id,attachment,texture.GetId(), level, layer);
}

void Framebuffer::GetPixelFromAttachment(const uint32_t attachmentIndex, const Vector2i position, const TextureFormat::TextureFormat textureFormat, const DataType::DataType dataType, void* const output) const
{
	Rhi::BindFrameBuffer(m_Id);
	Rhi::GetPixelFromAttachement(attachmentIndex, position, textureFormat, dataType, output);
	Rhi::UnbindFrameBuffer();
}

uint32_t Framebuffer::GetId() const
{
	return m_Id;
}
