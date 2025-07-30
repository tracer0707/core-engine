#include "ProjectManagerWindow.h"

#include "WindowList.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/FileInput.h"
#include "../Controls/Label.h"

namespace Editor
{
	ProjectManagerWindow::ProjectManagerWindow(WindowManager* parent) : Window(parent, PROJECT_MANAGER_WINDOW)
	{
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);
		layoutMain->setVerticalAlignment(LayoutAlignment::Center);
		layoutMain->setHorizontalAlignment(LayoutAlignment::Center);

		Label* label = new Label("Project location");
		FileInput* fileInput = new FileInput();

		layoutMain->addControl(label);
		layoutMain->addControl(fileInput);

		addControl(layoutMain);
	}

	ProjectManagerWindow::~ProjectManagerWindow()
	{
	}
}