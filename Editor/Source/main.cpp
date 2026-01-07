#include "Main/Editor.h"
#include "Main/ProjectManager.h"

#include <FreeImage.h>

int main(int argc, char* argv[])
{
	FreeImage_Initialise();

	Editor::ProjectManager projectManager;
	projectManager.run();

	if (projectManager.isForceClosed()) return 0;

	Editor::Editor editor;
	editor.setRootPath(projectManager.getSelectedProject());
	editor.run();

	FreeImage_DeInitialise();

	return 0;
}