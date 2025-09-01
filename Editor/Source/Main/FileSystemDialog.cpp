#include "FileSystemDialog.h"

#include <filesystem>
#include <iostream>

#include <Renderer/Renderer.h>
#include <Shared/IO.h>
#include <Shared/Path.h>
#include <Shared/List.h>
#include <Shared/String.h>

#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"
#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"

namespace Editor
{
    static void scanDisk(UString path, TreeView* treeView, TreeNode* rootNode)
    {
        std::string _path = ToStdString(path);

        TreeNode* _node = treeView->createNode();
        _node->setText(path);

        if (rootNode != nullptr)
        {
            rootNode->addControl(_node);
        }
        else
        {
            treeView->addControl(_node);
        }

        try
        {
            if (std::filesystem::is_directory(_path))
            {
                for (const auto& entry : std::filesystem::directory_iterator(_path))
                {
                    UString p = FromStdString(entry.path().generic_string());
                    if (Core::Path::isHiddenOrSystem(p)) continue;

                    TreeNode* _nodeChild = treeView->createNode();
                    _nodeChild->setText(entry.path().c_str());

                    _node->addControl(_nodeChild);
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Error accessing " << _path << ": " << e.what() << std::endl;
        }
    }

    FileSystemDialog::FileSystemDialog() : Window("File Dialog", 800, 400)
    {
        _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Start);
        _layout->getStyle().paddingX = 10;
        _layout->getStyle().paddingY = 10;

        Core::List<UString> _diskDrives = Core::IO::getDiskDrives();

        TreeView* _treeView = new TreeView();

        for (auto& d : _diskDrives)
        {
            scanDisk(d, _treeView, nullptr);
        }

        _layout->addControl(_treeView);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_layout);
    }

    FileSystemDialog::~FileSystemDialog()
    {
        delete _wnd;
        _wnd = nullptr;
    }

    void FileSystemDialog::update()
    {
        _layout->setWidth(_width);
        _layout->setHeight(_height);
    }

    void FileSystemDialog::render()
    {
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        _wnd->update("File Dialog", _width, _height);
        _renderer->endUI();
    }
}