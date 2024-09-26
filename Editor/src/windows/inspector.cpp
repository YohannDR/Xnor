#include "windows/inspector.hpp"

#include "imgui/imgui.h"
#include "reflection/type_renderer.hpp"

using namespace XnorEditor;

Inspector::Inspector(Editor* const editor)
    : UiWindow(editor, "Inspector")
{
}

void Inspector::Display()
{
    XnorCore::Entity* const ptr = m_Editor->data.selectedEntity;
    
    // Check if an entity was selected
    if (!ptr || m_Editor->IsSerializing() || m_Editor->IsDeserializing())
    {
        ImGui::Text("Nothing selected");
        return;
    }

    ImGui::PushID(ptr);

    const std::pair<void*, const char_t*> windowInfo = XnorCore::TypeRenderer::DisplayObject<XnorCore::Entity>(ptr);

    if (windowInfo.first != nullptr)
        m_Editor->OpenCreatedWindow(windowInfo.second, windowInfo.first);
    
    ImGui::PopID();
}
