#include "Main/EditorApp.h"
#include "Main/ProjectManager.h"

#include <FreeImage.h>

int main(int argc, char* argv[])
{
	FreeImage_Initialise();

	Editor::ProjectManager projectManager;
	projectManager.run();

	if (projectManager.isForceClosed()) return 0;

	Editor::EditorApp editor;
	editor.setRootPath(projectManager.getSelectedProject());
	editor.run();

	FreeImage_DeInitialise();

	return 0;
}