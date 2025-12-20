#include "ContentWindow.h"

#include <filesystem>
#include <imgui.h>

#include <Core/Content/Texture.h>

#include "WindowList.h"

#include "../../Utils/FileSystemUtils.h"
#include "../../System/ThumbCacheManager.h"
#include "../../System/ContentLoader.h"
#include "../../Main/Editor.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/SplitPanel.h"
#include "../Controls/Button.h"
#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"
#include "../Windows/WindowManager.h"

namespace fs = std::filesystem;

namespace Editor
{
    ContentWindow::ContentWindow(WindowManager* parent) : Window(parent, CONTENT_WINDOW)
    {
        LinearLayout* _mainLayout = new LinearLayout(LayoutDirection::Horizontal);
        LinearLayout* _leftPane = new LinearLayout(LayoutDirection::Vertical);
        _rightPane = new LinearLayout(LayoutDirection::Horizontal);

        SplitPanel* _splitPanel = new SplitPanel(SplitPanelDirection::Horizontal);
        _treeView = new TreeView();

        _leftPane->addControl(_treeView);
        _leftPane->setWidth(200);

        _splitPanel->addControl(_leftPane);
        _splitPanel->addControl(_rightPane);

        _mainLayout->addControl(_splitPanel);

        addControl(_mainLayout);

        _treeView->setOnSelectionChanged([=](Core::List<TreeNode*>& nodes) {
            if (nodes.count() == 0) return;
            setCurrentDir(nodes[0]->getStringTag(0));
        });
    }

    ContentWindow::~ContentWindow()
    {
        clearLoadedResources();
    }

    void ContentWindow::init()
    {
        rescanContent();
    }

    void ContentWindow::rescanContent()
    {
        _treeView->clear();
        FileSystemUtils::fsToTreeView(_contentDir, _treeView, nullptr, false, false);
    }

    void ContentWindow::setCurrentDir(Core::String path)
    {
        _rightPane->clear();

        clearLoadedResources();

        Core::List<std::filesystem::path> entries = FileSystemUtils::getPathEntries(path);

        for (auto& it : entries)
        {
            Button* thumbnail = new Button(it.filename().generic_string());
            Core::Texture* tex = _parent->getThumbCacheManager()->getOrCreate(it.generic_string());

            if (tex == nullptr)
            {
                tex = getIcon(it.extension().generic_string());
            }

            thumbnail->setImage(tex);
            thumbnail->setSize(64, 80);

            _rightPane->addControl(thumbnail);

            if (tex != nullptr)
            {
                _loadedThumbs.add(tex);
            }
        }
    }

    Core::Texture* ContentWindow::getIcon(Core::String ext)
    {
        Core::String iconName = Core::String::Empty;

        if (ext == ".ttf")
        {
            iconName = "font.png";
        }
        else if (ext == ".fbx")
        {
            iconName = "mesh.png";
        }

        if (iconName != Core::String::Empty)
        {
            return _parent->getContentLoader()->loadTextureFromFile(
                Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Icons/content", iconName), Core::TextureFormat::RGBA8,
                LoadMethod::Direct);
        }

        return nullptr;
    }

    void ContentWindow::clearLoadedResources()
    {
        for (auto it : _loadedThumbs)
        {
            _parent->getContentLoader()->unload(it);
        }

        _loadedThumbs.clear();
    }
} // namespace Editor