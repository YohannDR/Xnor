#pragma once

#include "definitions.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class FooterWindow : public UiWindow
{
public:
    static constexpr std::chrono::duration<float_t> FadeOutDuration{2.f};
    
    explicit FooterWindow(Editor* editor);

    ~FooterWindow() override;

    void Display() override;

private:
    XnorCore::Pointer<XnorCore::Texture> m_BuildTexture;
    
    XnorCore::Pointer<XnorCore::Texture> m_LastBuildSuccessTexture;
    XnorCore::Pointer<XnorCore::Texture> m_LastBuildWarningTexture;
    XnorCore::Pointer<XnorCore::Texture> m_LastBuildErrorTexture;

    ImFont* m_ProjectTextFont = nullptr;
    
    XnorCore::FileSystemWatcher m_ScriptsWatcher;

    bool_t scriptsUpToDate = true;
};

END_XNOR_EDITOR
