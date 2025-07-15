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
    Core::DeviceContext* ctx = new Core::DeviceContext();

    ProjectManager projectManager;
    int result = projectManager.run(ctx);
    if (result != 0)
    {
        fprintf(stderr, "Error: %s\n", getErrorString(result));
    }
    
    EditorBase editorBase;
    result = editorBase.run(ctx);
    if (result != 0)
    {
        fprintf(stderr, "Error: %s\n", getErrorString(result));
    }

    delete ctx;

    return 0;
}