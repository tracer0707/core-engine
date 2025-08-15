#include "Main/Editor.h"
#include "Main/ProjectManager.h"

int main(int argc, char* argv[])
{
    Editor::ProjectManager projectManager;
    projectManager.run();
    
    Editor::Editor editor;
    editor.run();

    return 0;
}