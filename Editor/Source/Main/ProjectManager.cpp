#include "ProjectManager.h"

#include <Renderer/Renderer.h>

#include "../Editor/Windows/WindowManager.h"
#include "../Editor/Windows/ProjectManagerWindow.h"

namespace Editor
{
    /* WINDOW */

    ProjectManager::MainWindow::MainWindow() : Window("Project Manager", 800, 400)
    {
        windowManager = new WindowManager();
        windowManager->setTime(_time);
        windowManager->setRenderer(_renderer);
        windowManager->setAssetManager(_assetManager);
        windowManager->setInputManager(_inputManager);

        ProjectManagerWindow* projectManagerWindow = windowManager->addWindow<ProjectManagerWindow*>();
        projectManagerWindow->setCanAcceptDocking(false);
        projectManagerWindow->setCanDock(false);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete windowManager;
        windowManager = nullptr;
    }

    void ProjectManager::MainWindow::update() {}

    void ProjectManager::MainWindow::render()
    {
        _renderer->setViewportSize(_width, _height);
        _renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        _renderer->beginUI();
        windowManager->update(_width, _height);
        _renderer->endUI();
    }

    /* PROJECT MANAGER */

    void ProjectManager::init()
    {
        wnd = new MainWindow();
        addWindow(wnd);
    }

    void ProjectManager::destroy()
    {
        wnd = nullptr;
    }
}