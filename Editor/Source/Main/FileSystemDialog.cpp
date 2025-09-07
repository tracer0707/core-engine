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
    static void scanPath(UString path, TreeView* treeView, TreeNode* rootNode)
    {
        std::string _path = ToStdString(path);
        auto fs_path = std::filesystem::path(_path);

        TreeNode* _node = treeView->createNode();
        if (rootNode != nullptr)
        {
            _node->setText(fs_path.filename().generic_string().c_str());
        }
        else
        {
            _node->setText(_path.c_str());
        }

        if (rootNode != nullptr)
        {
            rootNode->addControl(_node);
        }
        else
        {
            treeView->addControl(_node);
        }

        if (std::filesystem::is_directory(_path))
        {
            _node->setAlwaysShowOpenArrow(true);
            _node->setOnOpen([_path, treeView, _node](bool opened)
                {
                    if (opened)
                    {
                        try
                        {
                            for (const auto& entry : std::filesystem::directory_iterator(_path, std::filesystem::directory_options::skip_permission_denied))
                            {
                                UString path = FromStdString(entry.path().generic_string());
                                if (Core::Path::isHiddenOrSystem(path)) continue;

                                scanPath(path, treeView, _node);
                            }
                        }
                        catch (const std::filesystem::filesystem_error& e)
                        {
                            std::cerr << "Error accessing " << _path << ": " << e.what() << std::endl;
                        }
                    }
                });
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
            scanPath(d, _treeView, nullptr);
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