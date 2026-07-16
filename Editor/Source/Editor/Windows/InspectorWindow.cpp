#include "InspectorWindow.h"

#include <imgui.h>

#include "WindowList.h"

#include "Inspector/Inspector.h"

namespace Editor
{
	InspectorWindow::InspectorWindow(WindowManager* parent) : Window(parent, INSPECTOR_WINDOW) {}

	InspectorWindow::~InspectorWindow() {}

	void InspectorWindow::rebuild()
	{
		for (auto it : _controls)
		{
			it->clear();
			((Inspector*)it)->build();
		}
	}
} // namespace Editor