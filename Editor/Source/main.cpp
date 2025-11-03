#include "Main/Editor.h"
#include "Main/ProjectManager.h"

int main(int argc, char* argv[])
{
    Editor::ProjectManager projectManager;
    projectManager.run();

    if (projectManager.isForceClosed()) return 0;

    Editor::Editor editor;
    editor.setWorkingDir(projectManager.getSelectedProject());
    editor.run();

    return 0;
}