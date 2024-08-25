#include "InspectorWindow.h"

#include <imgui.h>

#include "../Controls/Button.h"

namespace Editor
{
	InspectorWindow::InspectorWindow() : Window("Inspector")
	{
		Button* btn = new Button();
		btn->setText("Test button");

		addControl(btn);
	}
}