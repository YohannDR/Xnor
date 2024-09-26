#include "windows/content_browser.hpp"

#include "file/directory.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "resource/resource_manager.hpp"

#define ASSETS_PATH "assets_internal/editor/ui/content_browser/"

using namespace XnorEditor;

void ContentBrowser::BeginDragDrop(const XnorCore::Pointer<XnorCore::File>& file)
{
    if (ImGui::BeginDragDropSource())
    {
        const XnorCore::Pointer<XnorCore::Resource> resource = file->GetResource();
        ImGui::SetDragDropPayload("ContentBrowserFile", &resource, sizeof(resource));
        ImGui::SetTooltip("%s", file->GetName().c_str());
        ImGui::EndDragDropSource();
    }
}

ContentBrowser::ContentBrowser(Editor* editor, XnorCore::Pointer<XnorCore::Directory>&& rootDirectory)
    : UiWindow(editor, "Content Browser")
    , m_RootDirectory(std::move(rootDirectory))
    , m_CurrentDirectory(m_RootDirectory)
{
    m_UnknownFileTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "file.png");
    m_DirectoryTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "directory.png");
    m_TextureFileTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "texture_file.png");
    m_ModelFileTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "model_file.png");
    m_FontFileTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "font_file.png");
    m_XmlFileTexture = XnorCore::ResourceManager::Get<XnorCore::Texture>(ASSETS_PATH "xml_file.png");
}

void ContentBrowser::Display()
{
    const ImVec2 available = ImGui::GetContentRegionAvail();

    ImGui::BeginChild("ContentBrowserLeftPanel", ImVec2(available.x * 0.5f, 0.f), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
    DisplayDirectoryHierarchy(static_cast<XnorCore::Pointer<XnorCore::Entry>>(m_RootDirectory));
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("ContentBrowserRightPanel");
    
    const std::string title = m_CurrentDirectory->GetPathString();
    const char_t* const titleStr = title.c_str();
    //available = ImGui::GetContentRegionAvail();
    const ImVec2 textSize = ImGui::CalcTextSize(titleStr);
    XnorCore::Utils::AlignImGuiCursor(textSize.x);
    ImGui::Text("%s", titleStr);

    bool_t entryHovered = false, entryClicked = false;
    for (const XnorCore::Pointer<XnorCore::Directory>& directory : m_CurrentDirectory->GetChildDirectories())
    {
        bool_t hovered = false, clicked = false;
        DisplayEntry(static_cast<XnorCore::Pointer<XnorCore::Entry>>(directory), m_DirectoryTexture, &hovered, &clicked);
        entryHovered |= hovered;
        entryClicked |= clicked;
    }

    for (const XnorCore::Pointer<XnorCore::File>& file : m_CurrentDirectory->GetChildFiles())
    {
        XnorCore::Pointer<XnorCore::Texture> correspondingTexture;
        if (m_FileTextureCache.contains(file))
        {
            correspondingTexture = m_FileTextureCache.at(file);
        }
        else
        {
            switch (file->GetType())  // NOLINT(clang-diagnostic-switch-enum)
            {
                case XnorCore::File::Type::Texture:
                    {
                        XnorCore::Pointer<XnorCore::Resource>&& correspondingResource = XnorCore::ResourceManager::Contains(file) ? file->GetResource() : nullptr;
                    
                        if (correspondingResource)
                            correspondingTexture = XnorCore::Utils::DynamicPointerCast<XnorCore::Texture>(correspondingResource);
                        else
                            correspondingTexture = m_TextureFileTexture;
                    }
                    break;
                
                case XnorCore::File::Type::Model:
                    correspondingTexture = m_ModelFileTexture;
                    break;
                
                case XnorCore::File::Type::Font:
                    correspondingTexture = m_FontFileTexture;
                    break;
                
                case XnorCore::File::Type::Xml:
                    correspondingTexture = m_XmlFileTexture;
                    break;
                
                default:
                    break;
            }

            if (correspondingTexture)
                m_FileTextureCache.emplace(file, correspondingTexture);
        }

        if (!correspondingTexture)
            correspondingTexture = m_UnknownFileTexture;
        
        bool_t hovered = false, clicked = false;
        DisplayEntry(static_cast<XnorCore::Pointer<XnorCore::Entry>>(file), correspondingTexture, &hovered, &clicked);
        entryHovered |= hovered;
        entryClicked |= clicked;
    }

    ImGui::EndChild();

    if (entryHovered == false)
        m_HoveredEntry = nullptr;
    if (entryClicked == false && ImGui::IsItemClicked())
        m_SelectedEntry = nullptr;

    HandleInputs();
}

const XnorCore::Pointer<XnorCore::Directory>& ContentBrowser::GetRootDirectory() const
{
    return m_RootDirectory;
}

void ContentBrowser::SetRootDirectory(const XnorCore::Pointer<XnorCore::Directory>& rootDirectory)
{
    m_RootDirectory = rootDirectory;

    // Make sure m_CurrentDirectory is not a parent directory of m_RootDirectory
    const std::string currentDirRelativeToRoot = relative(m_CurrentDirectory->GetPath(), m_RootDirectory->GetPath()).string();
    if (currentDirRelativeToRoot.find("..") != std::string::npos)
        m_CurrentDirectory = m_RootDirectory;
}

void ContentBrowser::DisplayDirectoryHierarchy(const XnorCore::Pointer<XnorCore::Entry>& entry)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;

    const XnorCore::Pointer<XnorCore::Directory> directory = XnorCore::Utils::DynamicPointerCast<XnorCore::Directory>(entry);
    const bool_t isDirectory = directory;
    
    if (!isDirectory)
        flags |= ImGuiTreeNodeFlags_Leaf;
    else if (entry == m_CurrentDirectory)
        flags |= ImGuiTreeNodeFlags_Selected;

    const bool_t isEntryToRename = m_EntryToRename == entry;

    std::string&& entryName = entry->GetName();
    const char* treeNodeName = isEntryToRename ? "##EntryToRename" : entryName.c_str();

    if (ImGui::TreeNodeEx(treeNodeName, flags))
    {
        if (m_EntryToRename == entry && m_IsEntryToRenameLeft)
        {
            ImGui::SameLine();
            RenameEntry(entry);
        }

        if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
            m_CurrentDirectory = directory;

        ContextMenu(entry, nullptr, true);
        
        if (isDirectory)
        {
            for (const XnorCore::Pointer<XnorCore::Entry>& childEntry : directory->GetChildEntries())
                DisplayDirectoryHierarchy(childEntry);
        }
        else
        {
            BeginDragDrop(XnorCore::Utils::DynamicPointerCast<XnorCore::File>(entry));
        }
        
        ImGui::TreePop();
    }
    else if (isDirectory && ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        m_CurrentDirectory = directory;
    }
}

void ContentBrowser::DisplayEntry(
    const XnorCore::Pointer<XnorCore::Entry>& entry,
    const XnorCore::Pointer<XnorCore::Texture>& texture,
    bool_t* const hovered,
    bool_t* const clicked
)
{
    const float_t oldCursorPos = ImGui::GetCursorPosX();

    bool_t pushedStyleVar = false;
    if (m_SelectedEntry == entry)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, static_cast<Vector4>(SelectedEntryColor));
        pushedStyleVar = true;
    }
    else if (m_HoveredEntry == entry)
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, static_cast<Vector4>(HoveredEntryColor));
        pushedStyleVar = true;
    }

    static constexpr float_t BaseChildSize = 64.f;

    const ImVec2 framePadding = ImGui::GetStyle().FramePadding;
    const float_t childWidth = BaseChildSize + framePadding.x * 2.f;
    const ImVec2 textSize = ImGui::CalcTextSize(entry->GetName().c_str(), nullptr, false, childWidth);
    
    ImGui::BeginChild(
        entry->GetPathString().c_str(),
        ImVec2(
            childWidth,
            BaseChildSize + textSize.y + framePadding.y * 2.f
        ),
        ImGuiChildFlags_None,
        ImGuiWindowFlags_NoScrollbar
    );
    
    const XnorCore::Pointer<XnorCore::File> file = XnorCore::Utils::DynamicPointerCast<XnorCore::File>(entry);
    if (file)
        BeginDragDrop(file);
    
    XnorCore::Utils::AlignImGuiCursor(64.f);
    ImGui::Image(XnorCore::Utils::IntToPointer<ImTextureID>(texture->GetId()), ImVec2(64.f, 64.f));
    
    XnorCore::Utils::AlignImGuiCursor(textSize.x);
    ImGui::PushTextWrapPos();
    if (m_EntryToRename == entry && !m_IsEntryToRenameLeft)
        RenameEntry(entry);
    else
        ImGui::Text("%s", entry->GetName().c_str());
    ImGui::PopTextWrapPos();
    
    ImGui::EndChild();

    ContextMenu(entry, entry->GetPathString().c_str(), false);

    if (pushedStyleVar)
        ImGui::PopStyleColor();

    if (ImGui::IsItemHovered())
    {
        m_HoveredEntry = entry;
        *hovered = true;
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        m_SelectedEntry = entry;
        *clicked = true;

        if (*hovered && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (file)
            {
                file->OpenFile();
            }
            else
            {
                // ReSharper disable once CppTooWideScope
                XnorCore::Pointer<XnorCore::Directory>&& directory = XnorCore::Utils::DynamicPointerCast<XnorCore::Directory>(entry);
                if (directory)
                {
                    m_CurrentDirectory = std::move(directory);
                    m_SelectedEntry = nullptr;
                }
            }
        }
    }

    ImGui::SameLine();
    
    if (ImGui::GetCursorPosX() - oldCursorPos > ImGui::GetContentRegionAvail().x)
        ImGui::Dummy(ImVec2());
}

void ContentBrowser::ContextMenu(XnorCore::Pointer<XnorCore::Entry> entry, const char_t* strId, const bool_t isLeftPanel)
{
    if (ImGui::BeginPopupContextItem(strId))
    {
        XnorCore::Pointer<XnorCore::File>&& file = XnorCore::Utils::DynamicPointerCast<XnorCore::File>(entry);
        if (file && ImGui::Selectable("Open"))
            file->OpenFile();

        if (ImGui::Selectable("Open in explorer"))
            entry->OpenInExplorer();

        if (ImGui::Selectable("Rename"))
        {
            m_EntryToRename = entry;
            m_IsEntryToRenameLeft = isLeftPanel;
        }

        //if (ImGui::Selectable("Delete"))

        ImGui::EndPopup();
    }
}

void ContentBrowser::RenameEntry(const XnorCore::Pointer<XnorCore::Entry>& entry)
{
    ImGui::SetKeyboardFocusHere();
        
    std::string name = entry->GetName();
    if (ImGui::InputText("##input", &name, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
    {
        m_EntryToRename->SetName(name);
        m_EntryToRename = nullptr;
    }

    if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        m_EntryToRename = nullptr;
}

void ContentBrowser::HandleInputs()
{
    if (!ImGui::GetIO().WantCaptureKeyboard)
        return;
    
    if (m_SelectedEntry && ImGui::IsKeyPressed(ImGuiKey_F2, false))
        m_EntryToRename = m_SelectedEntry;

    if (m_EntryToRename && ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        m_EntryToRename = nullptr;
}
