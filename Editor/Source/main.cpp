#include <System/DeviceContext.h>

#include "Main/EditorBase.h"
#include "Main/ProjectManager.h"

static const char* getErrorString(int error)
{
    switch (error)
    {
    case -1:
        return "Create window failed";
    default:
        return "Unknown error";
    }
}

int main(int argc, char* argv[])
{
    ProjectManager projectManager;
    int result = projectManager.run();
    if (result != 0)
    {
        fprintf(stderr, "Error: %s\n", getErrorString(result));
    }
    
    EditorBase editorBase;
    result = editorBase.run();
    if (result != 0)
    {
        fprintf(stderr, "Error: %s\n", getErrorString(result));
    }

    return 0;
}