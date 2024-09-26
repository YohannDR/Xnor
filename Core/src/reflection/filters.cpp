#include "reflection/filters.hpp"

#include "reflection/xnor_factory.hpp"
#include "world/world.hpp"

using namespace XnorCore;

Pointer<File> Filters::FilterFile(Pointer<File>* target)
{
    if (!ImGui::BeginPopupModal("File"))
        return nullptr;

    m_TextFilter.Draw();
    const std::vector<Pointer<File>> file = FileManager::FindAll<File>(
        [](Pointer<File> f) -> bool_t
        {
            return f->GetName().ends_with(".scene.xml");
        }
    );

    
    Pointer<File> r = nullptr;
    for (const Pointer<File>& res : file)
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

    // If the user pressed the Escape key or clicked outside the popup, close it
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return r;
}

Entity* Filters::FilterEntity(Entity** target)
{
    if (!ImGui::BeginPopupModal("Entity"))
        return nullptr;

    m_TextFilter.Draw();
    List<Entity*> entities = World::scene->GetEntities();
    entities.Sort();

    Entity* e = nullptr;
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        const char_t* const name = entities[i]->name.c_str();
        if (m_TextFilter.PassFilter(name) && ImGui::Selectable(name))
        {
            e = entities[i];
            break;
        }
    }

    if (e != nullptr)
    {
        *target = e;
        ImGui::CloseCurrentPopup();
    }

    // If the user pressed the Escape key or clicked outside the popup, close it
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return e;
}

Entity* Filters::FilterEntity()
{
    if (!ImGui::BeginPopupModal("Entity"))
        return nullptr;

    m_TextFilter.Draw();
    List<Entity*> entities = World::scene->GetEntities();
    entities.Sort();

    Entity* e = nullptr;
    for (size_t i = 0; i < entities.GetSize(); i++)
    {
        const char_t* const name = entities[i]->name.c_str();
        if (m_TextFilter.PassFilter(name) && ImGui::Selectable(name))
        {
            e = entities[i];
            break;
        }
    }

    if (e != nullptr)
    {
        ImGui::CloseCurrentPopup();
    }

    // If the user pressed the Escape key or clicked outside the popup, close it
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return e;
}

Component* Filters::FilterComponent(List<Component*>* target)
{
    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    m_TextFilter.Draw();

    Component* c = nullptr;

    List<std::string> names;
    XnorFactory::FindAllChildClasses<Component>(&names);
    names.Sort();

    ImGui::TextColored(static_cast<Vector4>(Color::Gray()), "C++ Components");

    for (size_t i = 0; i < names.GetSize(); i++)
    {
        if (m_TextFilter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = static_cast<Component*>(XnorFactory::CreateObject(names[i]));
            break;
        }
    }

    ImGui::Separator();

    names.Clear();
    DotnetReflection::GetScriptTypes(&names);
    names.Sort();

    ImGui::TextColored(static_cast<Vector4>(Color::Gray()), "C# Scripts");

    for (size_t i = 0; i < names.GetSize(); i++)
    {
        if (m_TextFilter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = ScriptComponent::New(names[i], DotnetRuntime::GetGameAssembly());
            break;
        }
    }

    if (c != nullptr)
    {
        target->Add(c);
        ImGui::CloseCurrentPopup();
    }

    // If the user pressed the Escape key or clicked outside the popup, close it
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
    return c;
}

Component* Filters::FilterComponent(Component** target)
{
    if (!ImGui::BeginPopupModal("Component"))
        return nullptr;

    m_TextFilter.Draw();

    Component* c = nullptr;

    List<std::string> names;
    XnorFactory::FindAllChildClasses<Component>(&names);
    names.Sort();

    for (size_t i = 0; i < names.GetSize(); i++)
    {
        if (m_TextFilter.PassFilter(names[i].c_str()) && ImGui::Selectable(names[i].c_str()))
        {
            c = static_cast<Component*>(XnorFactory::CreateObject(names[i]));
            break;
        }
    }

    if (c != nullptr)
    {
        *target = c;
    }

    ImGui::EndPopup();
    return c;
}

void Filters::BeginResourceFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Resource");
}

void Filters::BeginEntityFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Entity");
}

void Filters::BeginComponentFilter()
{
    m_TextFilter.Clear();
    ImGui::OpenPopup("Component");
}

void Filters::BeginFilter(const std::string& filterName)
{
    m_TextFilter.Clear();
    ImGui::OpenPopup(filterName.c_str());
}
