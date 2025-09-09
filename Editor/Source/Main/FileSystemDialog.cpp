#include "FileSystemDialog.h"

#include <filesystem>
#include <iostream>

#include <Renderer/Renderer.h>
#include <Shared/IO.h>
#include <Shared/Path.h>
#include <Shared/List.h>
#include <Shared/String.h>

#include "../Editor/Font.h"
#include "../Editor/Controls/ControlList.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/TextInput.h"
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
            _node->setText(fs_path.filename().generic_string().c_str());
        else
            _node->setText(_path.c_str());

        if (rootNode != nullptr)
            rootNode->addControl(_node);
        else
            treeView->addControl(_node);

        if (std::filesystem::is_directory(_path))
        {
            _node->setAlwaysShowOpenArrow(true);
            _node->setOnOpen([_path, treeView, _node](bool opened)
                {
                    if (opened)
                    {
                        try
                        {
                            Core::List<std::filesystem::path> fs;
                            for (const auto& entry : std::filesystem::directory_iterator(_path, std::filesystem::directory_options::skip_permission_denied))
                            {
                                fs.add(entry.path());
                            }

                            fs.sort([](std::filesystem::path& a, std::filesystem::path& b) -> bool {
                                bool isDirA = std::filesystem::is_directory(a);
                                bool isDirB = std::filesystem::is_directory(b);

                                std::string _a = a.generic_string();
                                std::string _b = b.generic_string();

                                std::transform(_a.begin(), _a.end(), _a.begin(), [](unsigned char c) { return std::tolower(c); });
                                std::transform(_b.begin(), _b.end(), _b.begin(), [](unsigned char c) { return std::tolower(c); });

                                if (isDirA && isDirB) return _a < _b;
                                if (isDirA) return 1;
                                if (isDirB) return 0;

                                return _a < _b;
                            });

                            for (const auto& entry : fs)
                            {
                                UString path = FromStdString(entry.generic_string());
                                if (Core::Path::isHiddenOrSystem(path)) continue;

                                scanPath(path, treeView, _node);
                            }
                        }
                        catch (const std::filesystem::filesystem_error& e)
                        {
                            std::cerr << "Error accessing " << _path << ": " << e.what() << std::endl;
                        }
                    }
                    else
                    {
                        _node->clear();
                    }
                });
        }
    }

    FileSystemDialog::FileSystemDialog() : Core::Window("File Dialog", 800, 400)
    {
        _mainFont = new Font(Core::Path::combine(Core::Path::getExePath(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();

        _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Start);
        _layout->getStyle().paddingX = 10;
        _layout->getStyle().paddingY = 10;

        _topLayout = new LinearLayout(LayoutDirection::Vertical);
        _topLayout->setVerticalAlignment(LayoutAlignment::Start);
        _topLayout->setHorizontalAlignment(LayoutAlignment::Start);

        Core::List<UString> _diskDrives = Core::IO::getDiskDrives();

        TreeView* _treeView = new TreeView();

        for (auto& d : _diskDrives)
        {
            scanPath(d, _treeView, nullptr);
        }

        _topLayout->addControl(_treeView);

        _bottomLayout = new LinearLayout(LayoutDirection::Horizontal);
        _bottomLayout->setHorizontalAlignment(LayoutAlignment::Center);

        TextInput* selectedFile = new TextInput();
        Button* okBtn = new Button("OK");
        Button* cancelBtn = new Button("Cancel");

        _bottomLayout->addControl(selectedFile);
        _bottomLayout->addControl(okBtn);
        _bottomLayout->addControl(cancelBtn);

        _layout->addControl(_topLayout);
        _layout->addControl(_bottomLayout);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_layout);
    }

    FileSystemDialog::~FileSystemDialog()
    {
        delete _mainFont;
        delete _wnd;

        _mainFont = nullptr;
        _wnd = nullptr;
    }

    void FileSystemDialog::update()
    {
        _layout->setWidth(_width);
        _layout->setHeight(_height);

        _topLayout->setWidth(_width - 20);
        _topLayout->setHeight(_height - 45);
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