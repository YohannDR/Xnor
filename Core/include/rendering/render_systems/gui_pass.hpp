#pragma once
#include <rendering/buffer/vbo.hpp>

#include "core.hpp"
#include "rendering/buffer/vao.hpp"
#include "resource/shader.hpp"
#include "scene/scene.hpp"
#include "scene/component/button.hpp"
#include "scene/component/text_component.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class GuiPass
{
public:
    XNOR_ENGINE GuiPass() = default;

    XNOR_ENGINE ~GuiPass() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(GuiPass)
    
    XNOR_ENGINE void RenderGui(const Scene& scene, const Vector2i& viewportSize) const;

    XNOR_ENGINE void Init();
    
private:
    Pointer<Shader> m_GuiShader;

    Pointer<Shader> m_FontShader;

    Pointer<Model> m_Quad;

    mutable Vao m_FontQuadVao;
    
    mutable Vbo m_FontQuadVbo;
    
    mutable std::vector<const Image*> m_Images;
    
    mutable std::vector<const TextComponent*> m_TextComponents;

    XNOR_ENGINE void InitSliderQuad();

    XNOR_ENGINE void ResetQuad() const;

    XNOR_ENGINE void RenderText() const;
    
    XNOR_ENGINE void RenderImage(Vector2 viewPortSize) const;
    
    XNOR_ENGINE void DrawImage(const Image* imageComponent,Vector2 viewPortSize) const;
};

END_XNOR_CORE
