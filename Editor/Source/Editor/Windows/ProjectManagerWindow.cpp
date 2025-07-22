#include "ProjectManagerWindow.h"

#include "WindowList.h"

#include "../Controls/LinearLayout.h"

namespace Editor
{
	ProjectManagerWindow::ProjectManagerWindow(WindowManager* parent) : Window(parent, PROJECT_MANAGER_WINDOW)
	{
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);
	}

	ProjectManagerWindow::~ProjectManagerWindow()
	{
	}
}