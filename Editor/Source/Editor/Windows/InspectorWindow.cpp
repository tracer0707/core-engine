#include "InspectorWindow.h"

#include <imgui.h>

#include "WindowList.h"

#include "Inspector/Inspector.h"

namespace Editor
{
	InspectorWindow::InspectorWindow(WindowManager* parent) : Window(parent, INSPECTOR_WINDOW) {}

	void InspectorWindow::setInspector(Inspector* value)
	{
		if (_currentInspector != nullptr)
		{
			clear();
			delete _currentInspector;
			_currentInspector = nullptr;
		}

		if (value != nullptr)
		{
			_currentInspector = value;
			addControl(_currentInspector->build());
		}
	}
} // namespace Editor