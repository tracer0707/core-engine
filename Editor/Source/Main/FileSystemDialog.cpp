#include "FileSystemDialog.h"

#include <Renderer/Renderer.h>

#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"

namespace Editor
{
    FileSystemDialog::FileSystemDialog() : Window("File Dialog", 800, 400)
    {
        LinearLayout* _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Center);
        _layout->setHorizontalAlignment(LayoutAlignment::Center);

        Label* label = new Label("Project location");

        _layout->addControl(label);

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