#include "ProjectManager.h"

#include <Shared/Path.h>
#include <Renderer/Renderer.h>

#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"

namespace Editor
{
    /* WINDOW */

    ProjectManager::MainWindow::MainWindow(Core::Application* app) : Window("Project Manager", 900, 500)
    {
        _app = app;

        LinearLayout* _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Center);
        _layout->setHorizontalAlignment(LayoutAlignment::Center);

        Label* label = new Label("Project location");
        FileInput* fileInput = new FileInput(_app);

        _layout->addControl(label);
        _layout->addControl(fileInput);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_layout);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete _wnd;

        _wnd = nullptr;
        _app = nullptr;
    }

    void ProjectManager::MainWindow::update() {}

    void ProjectManager::MainWindow::render()
    {
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        _wnd->update("Project Manager", _width, _height);
        _renderer->endUI();
    }

    /* PROJECT MANAGER */

    void ProjectManager::init()
    {
        _wnd = new MainWindow(this);
        addWindow(_wnd);

        _mainFont = new Font(Core::Path::combine(Core::Path::getExePath(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();
    }

    void ProjectManager::destroy()
    {
        delete _mainFont;

        _mainFont = nullptr;
        _wnd = nullptr;
    }
}