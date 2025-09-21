#include "ProjectManager.h"

#include <filesystem>

#include <Core/Shared/Path.h>
#include <Core/Renderer/Renderer.h>

#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"
#include "../Editor/Controls/Button.h"

namespace Editor
{
    /* WINDOW */

    ProjectManager::MainWindow::MainWindow(Core::Application* app) : Window(app, "Project Manager", 700, 500)
    {
        LinearLayout* _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Center);

        _layout->getStyle().paddingX = 20;
        _layout->getStyle().paddingY = 20;

        LinearLayout* _buttonsLayout = new LinearLayout();
        _buttonsLayout->setHorizontalAlignment(LayoutAlignment::Center);

        Label* label = new Label("Project location");
        FileInput* fileInput = new FileInput(_application);
        Button* openBtn = new Button("Open");
        Button* quitBtn = new Button("Quit");

        _buttonsLayout->addControl(openBtn);
        _buttonsLayout->addControl(quitBtn);

        _layout->addControl(label);
        _layout->addControl(fileInput);
        _layout->addControl(_buttonsLayout);

        openBtn->setOnClick([]()
        {

        });

        quitBtn->setOnClick([app]()
        {
            app->stop(true);
        });

        _wnd = new FullscreenWindow();
        _wnd->addControl(_layout);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete _wnd;
        _wnd = nullptr;
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

        _wnd->setOnClose([this]()
        {
            _isRunning = false;
            _forceClosed = true;
        });

        _mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();
    }

    void ProjectManager::destroy()
    {
        delete _mainFont;

        _mainFont = nullptr;
        _wnd = nullptr;
    }
}