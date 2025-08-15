#include "ProjectManagerWindow.h"

#include <Renderer/Renderer.h>

#include "WindowList.h"
#include "WindowManager.h"

#include "../Controls/LinearLayout.h"
#include "../Controls/FileInput.h"
#include "../Controls/Label.h"

namespace Editor
{
	ProjectManagerWindow::ProjectManagerWindow(WindowManager* parent) : Window(parent, PROJECT_MANAGER_WINDOW)
	{
		setHasTitle(false);
		setCanDock(false);
		setCanAcceptDocking(false);
		setCanResize(false);
		setCanMove(false);

		setPositionX(0);
		setPositionY(0);

		_style.borderSize = 0.0f;

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

	void ProjectManagerWindow::onUpdate()
	{
		setWidth(_parent->getRenderer()->getWidth());
		setHeight(_parent->getRenderer()->getHeight());
	}
}