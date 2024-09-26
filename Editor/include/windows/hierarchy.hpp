#pragma once

#include "definitions.hpp"
#include "windows/ui_window.hpp"

#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

class Hierarchy final : public UiWindow
{
public:
    using UiWindow::UiWindow;

    explicit Hierarchy(Editor* editor);
    
    void Display() override;

private:
    XnorCore::Entity* m_EntityToDelete = nullptr;
    XnorCore::Entity* m_EntityToRename = nullptr;
    bool_t m_ClickedOnEntity = false;
    
    void DisplayEntity(XnorCore::Scene& scene, XnorCore::Entity* entity);
    void DisplayEntityContextMenu(XnorCore::Scene& scene, XnorCore::Entity* entity);
    void DisplayEntityRenaming(XnorCore::Entity* entity);

    void ProcessEntityDragDrop(XnorCore::Entity* entity);
    void ProcessEntitySelection(XnorCore::Entity* entity);
    void ProcessEntityDoubleClick();
    
    void CheckDeleteEntity(XnorCore::Scene& scene);
    void CheckUnselectEntity();

    [[nodiscard]]
    ImGuiTreeNodeFlags GetEntityNodeFlags(const XnorCore::Entity* entity) const;

    [[nodiscard]]
    bool_t IsRenamingEntity(const XnorCore::Entity* entity) const;
    [[nodiscard]]
    bool_t IsEntitySelected(const XnorCore::Entity* entity) const;
};

END_XNOR_EDITOR
