#include "windows/hierarchy.hpp"

#include "world/world.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "scene/scene.hpp"

using namespace XnorEditor;

Hierarchy::Hierarchy(Editor* editor)
    : UiWindow(editor, "Scene Graph")
{
}

void Hierarchy::Display()
{
    XnorCore::Scene& scene = *XnorCore::World::scene;
  
    const XnorCore::List<XnorCore::Entity*>& entities = scene.GetEntities();

    const bool_t serializing = m_Editor->IsSerializing() || m_Editor->IsDeserializing();

    if (serializing)
        ImGui::BeginDisabled();

    const std::string treeNodeName = m_Editor->data.currentScene.IsValid() ? m_Editor->data.currentScene->GetNameNoExtension() : "Entities";
    const std::filesystem::path path = treeNodeName;

    if (ImGui::TreeNodeEx(path.stem().generic_string().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Selectable("Add entity"))
                scene.CreateEntity("Entity", nullptr);

            ImGui::EndPopup();
        }
        
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
            
            if (payload)
            {
                XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

                dragged->SetParent(nullptr);
            }
            
            ImGui::EndDragDropTarget();
        }

        for (size_t i = 0; i < entities.GetSize(); i++)
        {
            if (!entities[i]->HasParent())
                DisplayEntity(scene, entities[i]);
        }
        
        ImGui::TreePop();
    }
    
    CheckDeleteEntity(scene);
    CheckUnselectEntity();

    if (serializing)
        ImGui::EndDisabled();
}

void Hierarchy::DisplayEntity(XnorCore::Scene& scene, XnorCore::Entity* const entity)
{
    ImGui::PushID(static_cast<int32_t>(entity->GetGuid().GetData1()));
    
    const char_t* name = entity->name.c_str();
    const bool_t isRenaming = IsRenamingEntity(entity);
    
    const ImGuiTreeNodeFlags flags = GetEntityNodeFlags(entity);
    
    if (isRenaming)
        name = "##renaming";
    
    if (ImGui::TreeNodeEx(name, flags))
    {
        if (isRenaming)
        {
            DisplayEntityRenaming(entity);
        }
        else
        {
            ProcessEntitySelection(entity);
            ProcessEntityDoubleClick();
            
            DisplayEntityContextMenu(scene, entity);
            ProcessEntityDragDrop(entity);
        }
        
        for (size_t i = 0; i < entity->GetChildCount(); i++)
            DisplayEntity(scene, entity->GetChild(i));

        ImGui::TreePop();
    }
    else
    {
        ProcessEntitySelection(entity);
        ProcessEntityDoubleClick();
    }

    ImGui::PopID();
}

void Hierarchy::DisplayEntityContextMenu(XnorCore::Scene& scene, XnorCore::Entity* const entity)
{
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Add child"))
            scene.CreateEntity("Entity", entity);

        if (ImGui::Selectable("Add parent"))
        {
            XnorCore::Entity* const e = scene.CreateEntity("Entity", entity->GetParent());
            e->AddChild(entity);
        }

        if (ImGui::Selectable("Parent to root"))
            entity->SetParent(nullptr);
                    
        if (ImGui::Selectable("Rename"))
            m_EntityToRename = entity;
                    
        if (ImGui::Selectable("Duplicate"))
            m_Editor->data.selectedEntity = entity->Clone();

        if (ImGui::Selectable("Delete"))
            m_EntityToDelete = entity;

        ImGui::EndPopup();
    }
}

void Hierarchy::DisplayEntityRenaming(XnorCore::Entity* const entity)
{
    ImGui::SameLine();
    ImGui::SetKeyboardFocusHere();

    if (ImGui::InputText("##input", &entity->name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll))
        m_EntityToRename = nullptr;

    if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        m_EntityToRename = nullptr;
}

void Hierarchy::ProcessEntityDragDrop(XnorCore::Entity* const entity)
{
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("HierarchyEntity", &entity, sizeof(XnorCore::Entity*));
        ImGui::SetTooltip("%s", entity->name.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        // ReSharper disable once CppTooWideScope
        const ImGuiPayload* const payload = ImGui::AcceptDragDropPayload("HierarchyEntity");
                
        if (payload)
        {
            XnorCore::Entity* const dragged = *static_cast<XnorCore::Entity**>(payload->Data);

            if (!dragged->IsParentOf(entity))
                dragged->SetParent(entity);
        }
                
        ImGui::EndDragDropTarget();
    }
}

void Hierarchy::ProcessEntitySelection(XnorCore::Entity* const entity)
{
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
    {
        m_Editor->data.selectedEntity = entity;
        m_ClickedOnEntity = true;
    }
}

void Hierarchy::ProcessEntityDoubleClick()
{
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        m_Editor->data.gotoObject = true;
    }
}

void Hierarchy::CheckDeleteEntity(XnorCore::Scene& scene)
{
    if (m_EntityToDelete)
    {
        scene.DestroyEntity(m_EntityToDelete);
        m_EntityToDelete = nullptr;

        if (!scene.HasEntity(m_Editor->data.selectedEntity))
            m_Editor->data.selectedEntity = nullptr;
    }
}

void Hierarchy::CheckUnselectEntity()
{
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_ClickedOnEntity)
        m_Editor->data.selectedEntity = nullptr;
    
    m_ClickedOnEntity = false;
}

ImGuiTreeNodeFlags Hierarchy::GetEntityNodeFlags(const XnorCore::Entity* const entity) const
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!entity->HasChildren())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (IsEntitySelected(entity) || IsRenamingEntity(entity))
        flags |= ImGuiTreeNodeFlags_Selected;

    return flags;
}

bool_t Hierarchy::IsRenamingEntity(const XnorCore::Entity* entity) const
{
    return m_EntityToRename == entity;
}

bool_t Hierarchy::IsEntitySelected(const XnorCore::Entity* entity) const
{
    return m_Editor->data.selectedEntity == entity;
}
