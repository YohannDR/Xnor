#include  "rendering/render_pass.hpp"
#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

RenderPass::RenderPass(const std::vector<RenderTargetInfo>& attachments)
    : renderPassAttachments(attachments)
{
}

void RenderPass::BeginRenderPass(const RenderPassBeginInfo& beginInfo) const
{
    if (!beginInfo.frameBuffer)
    {
        Logger::LogFatal("BeginRenderPass framebuffer is invalid");
        throw std::runtime_error("BeginRenderPass framebuffer is invalid");
    }

    Rhi::BeginRenderPassInternal(beginInfo);
}

void RenderPass::EndRenderPass() const
{
    Rhi::EndRenderPass();
}
