#include "FileSystemDialog.h"

#include <Renderer/Renderer.h>
#include <Shared/IO.h>
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
    FileSystemDialog::FileSystemDialog() : Window("File Dialog", 800, 400)
    {
        LinearLayout* _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Start);
        _layout->getStyle().paddingX = 10;
        _layout->getStyle().paddingY = 10;

        Core::List<UString> _diskDrives = Core::IO::getDiskDrives();

        TreeView* _treeView = new TreeView();

        for (auto& d : _diskDrives)
        {
            TreeNode* _node = _treeView->createNode();
            _node->setText(d);
            _treeView->addControl(_node);
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

    void FileSystemDialog::update() {}

    void FileSystemDialog::render()
    {
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        _wnd->update("File Dialog", _width, _height);
        _renderer->endUI();
    }
}