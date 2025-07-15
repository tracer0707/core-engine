#include "ProjectManager.h"

#include <System/DeviceContext.h>
#include <Renderer/Renderer.h>

int ProjectManager::run(Core::DeviceContext* context)
{
    ctx = context;

    bool result = init();
    if (result != 0)
    {
        return result;
    }

    loop();
    destroy();

    return 0;
}

int ProjectManager::init()
{
    if (ctx->createWindow("Project Manager", 1366, 768) != 0)
        return -1;

    isRunning = true;

    return 0;
}

void ProjectManager::loop()
{
    while (isRunning)
    {
        ctx->updateWindow(isRunning);

        //** Render UI begin **//
        int width = ctx->getWindowWidth();
        int height = ctx->getWindowHeight();

        Core::Renderer::singleton()->setViewportSize(width, height);
        Core::Renderer::singleton()->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

        Core::Renderer::singleton()->beginUI();
        //Editor::WindowManager::singleton()->update(width, height);
        Core::Renderer::singleton()->endUI();
        //** Render UI end **//

        ctx->swapWindow();
    }
}

void ProjectManager::destroy()
{
    ctx->destroyWindow();
}