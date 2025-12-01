#include "FileSystemDialog.h"

#include <filesystem>

#include <Core/Renderer/Renderer.h>
#include <Core/Shared/IO.h>
#include <Core/Shared/Path.h>
#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include "../Utils/FileSystemUtils.h"
#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/TextInput.h"
#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"

namespace Editor
{
    FileSystemDialog::FileSystemDialog(Core::Application* app) : Core::Window(app, "File Dialog", 800, 400)
    {
        _mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();

        _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Start);
        _layout->setStretchX(true);
        _layout->getStyle().paddingX = 10;
        _layout->getStyle().paddingY = 10;

        _topLayout = new LinearLayout(LayoutDirection::Vertical);
        _topLayout->setVerticalAlignment(LayoutAlignment::Start);
        _topLayout->setHorizontalAlignment(LayoutAlignment::Start);
        _topLayout->setStretchX(true);

        _treeView = new TreeView();

        rescanFs();

        _topLayout->addControl(_treeView);

        _bottomLayout = new LinearLayout(LayoutDirection::Horizontal);
        _bottomLayout->setHorizontalAlignment(LayoutAlignment::Center);

        TextInput* selectedPath = new TextInput();
        Button* okBtn = new Button("OK");
        Button* cancelBtn = new Button("Cancel");

        okBtn->setEnabled(false);

        _bottomLayout->addControl(selectedPath);
        _bottomLayout->addControl(okBtn);
        _bottomLayout->addControl(cancelBtn);

        _layout->addControl(_topLayout);
        _layout->addControl(_bottomLayout);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_layout);

        _treeView->setOnSelectionChanged([=](Core::List<TreeNode*> lst) {
            if (lst.count() > 0)
            {
                selectedPath->setText(lst.get(0)->getStringTag(0));
                okBtn->setEnabled(true);
            }
            else
            {
                selectedPath->setText(Core::String::Empty);
                okBtn->setEnabled(false);
            }
        });

        selectedPath->setOnTextChanged([=](Core::String value) {
            auto path = std::filesystem::path(value.std_str());
            bool checkFile = _showFiles || std::filesystem::is_directory(path);
            okBtn->setEnabled(std::filesystem::exists(path) && checkFile);
        });

        cancelBtn->setOnClick([this]() { close(); });

        okBtn->setOnClick([=]() {
            if (selectedPath->getText() != Core::String::Empty && _onPathSelected != nullptr)
            {
                _onPathSelected(selectedPath->getText());
            }

            close();
        });
    }

    FileSystemDialog::~FileSystemDialog()
    {
        delete _mainFont;
        delete _wnd;

        _mainFont = nullptr;
        _wnd = nullptr;
    }

    void FileSystemDialog::setShowFiles(bool value)
    {
        _showFiles = value;
        rescanFs();
    }

    void FileSystemDialog::rescanFs()
    {
        _treeView->clear();
        Core::List<Core::String> _diskDrives = FileSystemUtils::getDiskDrives();
        for (auto& d : _diskDrives)
        {
            FileSystemUtils::fsToTreeView(d, _treeView, nullptr, _showFiles, true);
        }
    }

    void FileSystemDialog::update()
    {
        _layout->setHeight(_height);
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
} // namespace Editor