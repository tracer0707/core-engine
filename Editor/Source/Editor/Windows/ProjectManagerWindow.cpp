#include "ProjectManagerWindow.h"

#include "WindowList.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/FileInput.h"

namespace Editor
{
	ProjectManagerWindow::ProjectManagerWindow(WindowManager* parent) : Window(parent, PROJECT_MANAGER_WINDOW)
	{
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Horizontal);
		layoutMain->setAlignmentVertical(LayoutAlignment::Center);
		layoutMain->setAlignmentHorizontal(LayoutAlignment::Center);

		FileInput* fileInput = new FileInput();

		layoutMain->addControl(fileInput);

		addControl(layoutMain);
	}

	ProjectManagerWindow::~ProjectManagerWindow()
	{
	}
}