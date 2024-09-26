#pragma once

#include "resource/resource_manager.hpp"

BEGIN_XNOR_CORE

template <Concepts::ResourceT T>
Pointer<T> Filters::FilterResources(Pointer<T>* const target)
{
    if (!ImGui::BeginPopupModal("Resource"))
        return nullptr;

    m_TextFilter.Draw();
    std::vector<Pointer<T>> resources = ResourceManager::FindAll<T>(
        [&](const Pointer<T> r) -> bool_t
        {
            const std::string& name = r->GetName();

            if (name.starts_with("assets_internal"))
                return false;

            return m_TextFilter.PassFilter(name.c_str());
        }
    );

    Pointer<T> r = nullptr;
    for (const Pointer<T>& res : resources)
    {
        if (ImGui::Selectable(res->GetName().c_str()))
        {
            r = res;
            break;
        }
    }

    if (r != nullptr)
    {
        *target = r;
        ImGui::CloseCurrentPopup();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return r;
}

END_XNOR_CORE
