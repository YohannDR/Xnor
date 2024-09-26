#include "editing/picking_strategy.hpp"
#include "editor.hpp"
#include "rendering/rhi_typedef.hpp"
#include "resource/resource_manager.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

PickingStrategy::PickingStrategy(Editor* editor)
    : m_Editor(editor)
{
    const Vector2i windowSize = XnorCore::Window::GetSize();
    InitRendering(windowSize);
    
    m_PickingShaderStatic = XnorCore::ResourceManager::Get<XnorCore::Shader>("picking_shader");
    m_PickingShaderStatic->CreateInInterface();

    m_PickingShaderSkinned = XnorCore::ResourceManager::Get<XnorCore::Shader>("picking_shader_skinned");
    m_PickingShaderSkinned->CreateInInterface();
}

PickingStrategy::~PickingStrategy()
{
    DestroyRendering();
}

void PickingStrategy::ResizeHandle(const Vector2i newSize)
{
    if (m_ColorAttachment->GetSize() != newSize)
    {
        DestroyRendering();
        InitRendering(newSize);
    }
}

bool_t PickingStrategy::GetEntityFromScreen(const Vector2i pixelPos, XnorCore::Scene& scene, const XnorCore::Camera& pointOfView,
    XnorCore::Entity** entity)
{
    if (!m_Editor)
        return false;

    if (!frameBuffer)
        return false;

    // Draw
    m_PickingShaderStatic->Use();
    const XnorCore::RenderPassBeginInfo beginInfo =
    {
        .frameBuffer = frameBuffer,
        .renderAreaOffset = { 0, 0 },
        .renderAreaExtent = m_ColorAttachment->GetSize(),
        .clearBufferFlags = static_cast<XnorCore::BufferFlag::BufferFlag>(XnorCore::BufferFlag::ColorBit | XnorCore::BufferFlag::DepthBit),
        .clearColor = Vector4::Zero()
    };

    if (XnorCore::World::scene != nullptr)
        m_Editor->renderer.ZPass(*XnorCore::World::scene, pointOfView, beginInfo, m_ColorPass, m_PickingShaderStatic, m_PickingShaderSkinned, true);

    m_PickingShaderStatic->Unuse();

    float_t getValue = 0.f;
    frameBuffer->GetPixelFromAttachment(0, pixelPos, XnorCore::TextureFormat::Red, XnorCore::DataType::Float, &getValue);

    if (getValue == 0.f)
        return false;

    // - 1 cause in render wee need to do +1 to avoid the black color of the attachment be a valid index  
    const uint32_t entityIndex = static_cast<uint32_t>(getValue) - 1;

    const XnorCore::List<XnorCore::Entity*>& entities = scene.GetEntities();

    if (entityIndex < entities.GetSize())
    {
        *entity = entities[entityIndex];
        return true;
    }
    
    return false;
}

void PickingStrategy::DestroyRendering() const
{
    delete frameBuffer;
    delete m_ColorAttachment;
    delete m_DepthAttachement;
}

void PickingStrategy::InitRendering(const Vector2i size)
{
    frameBuffer = new XnorCore::Framebuffer();
    m_ColorAttachment = new XnorCore::Texture(XnorCore::TextureInternalFormat::R32F, size);
    m_DepthAttachement = new XnorCore::Texture(XnorCore::TextureInternalFormat::DepthComponent16, size);

    const std::vector<XnorCore::RenderTargetInfo> attachementsType =
    {
        {
            XnorCore::Attachment::Color00
        },
        {
            XnorCore::Attachment::Depth
        }
    };

    m_ColorPass = XnorCore::RenderPass(attachementsType);
    const std::vector<const XnorCore::Texture*> targets = { m_ColorAttachment, m_DepthAttachement };
    frameBuffer->AttachTextures(m_ColorPass, targets);
}
