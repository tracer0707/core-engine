#include "ProjectManager.h"

#include <Renderer/Renderer.h>

#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/ProjectManagerWindow.h"

namespace Editor
{
    /* WINDOW */

    ProjectManager::MainWindow::MainWindow() : Window("Project Manager", 800, 400)
    {
        _windowManager = new WindowManager();
        _windowManager->setTime(_time);
        _windowManager->setRenderer(_renderer);
        _windowManager->setAssetManager(_assetManager);
        _windowManager->setInputManager(_inputManager);

        ProjectManagerWindow* projectManagerWindow = _windowManager->addWindow<ProjectManagerWindow*>();
        projectManagerWindow->setCanAcceptDocking(false);
        projectManagerWindow->setCanDock(false);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete _windowManager;
        _windowManager = nullptr;
    }

    void ProjectManager::MainWindow::update() {}

    void ProjectManager::MainWindow::render()
    {
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        _windowManager->update(_width, _height);
        _renderer->endUI();
    }

    /* PROJECT MANAGER */

    void ProjectManager::init()
    {
        _wnd = new MainWindow();
        addWindow(_wnd);
    }

    void ProjectManager::destroy()
    {
        _wnd = nullptr;
    }
}