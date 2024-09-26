#pragma once

#include <filesystem>

#include "definitions.hpp"
#include "ui_window.hpp"

BEGIN_XNOR_EDITOR

class ContentBrowser : public UiWindow
{
    static void BeginDragDrop(const XnorCore::Pointer<XnorCore::File>& file);
    
public:
    static constexpr XnorCore::Color SelectedEntryColor = XnorCore::Color(0.5f, 0.5f, 0.5f);
    static constexpr XnorCore::Color HoveredEntryColor = SelectedEntryColor * 0.5f;
    static constexpr uint8_t MaxEntryTextLines = 3;
    
    using UiWindow::UiWindow;

    ContentBrowser(Editor* editor, XnorCore::Pointer<XnorCore::Directory>&& rootDirectory);
    
    void Display() override;

    [[nodiscard]]
    const XnorCore::Pointer<XnorCore::Directory>& GetRootDirectory() const;

    void SetRootDirectory(const XnorCore::Pointer<XnorCore::Directory>& rootDirectory);

private:
    XnorCore::Pointer<XnorCore::Directory> m_RootDirectory;
    XnorCore::Pointer<XnorCore::Directory> m_CurrentDirectory;
    
    XnorCore::Pointer<XnorCore::Entry> m_SelectedEntry;
    XnorCore::Pointer<XnorCore::Entry> m_HoveredEntry;
    XnorCore::Pointer<XnorCore::Entry> m_EntryToRename;
    // Whether the entry to rename is on the left or right panel
    bool_t m_IsEntryToRenameLeft = false;

    XnorCore::Pointer<XnorCore::Texture> m_DirectoryTexture;
    // Texture used to display unknown file types
    XnorCore::Pointer<XnorCore::Texture> m_UnknownFileTexture;
    XnorCore::Pointer<XnorCore::Texture> m_TextureFileTexture;
    XnorCore::Pointer<XnorCore::Texture> m_ModelFileTexture;
    XnorCore::Pointer<XnorCore::Texture> m_FontFileTexture;
    XnorCore::Pointer<XnorCore::Texture> m_XmlFileTexture;

    // ResourceManager textures stored here for faster lookup
    std::unordered_map<XnorCore::Pointer<XnorCore::File>, XnorCore::Pointer<XnorCore::Texture>> m_FileTextureCache;

    void DisplayDirectoryHierarchy(const XnorCore::Pointer<XnorCore::Entry>& entry);

    void DisplayEntry(const XnorCore::Pointer<XnorCore::Entry>& entry, const XnorCore::Pointer<XnorCore::Texture>& texture, bool_t* hovered, bool_t* clicked);

    void ContextMenu(XnorCore::Pointer<XnorCore::Entry> entry, const char_t* strId, bool_t isLeftPanel);

    void RenameEntry(const XnorCore::Pointer<XnorCore::Entry>& entry);

    void HandleInputs();
};

END_XNOR_EDITOR
