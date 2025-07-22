#include "InspectorWindow.h"

#include <imgui.h>

#include "WindowList.h"

#include "../Controls/Button.h"

namespace Editor
{
	InspectorWindow::InspectorWindow(WindowManager* parent) : Window(parent, INSPECTOR_WINDOW)
	{
		Button* btn = new Button();
		btn->setText("Test button");

		addControl(btn);
	}
}