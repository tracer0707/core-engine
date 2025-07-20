#include "InspectorWindow.h"

#include <imgui.h>

#include "../Controls/Button.h"

namespace Editor
{
	const char* InspectorWindow::NAME = "Inspector";

	InspectorWindow::InspectorWindow(WindowManager* parent) : Window(parent, NAME)
	{
		Button* btn = new Button();
		btn->setText("Test button");

		addControl(btn);
	}
}