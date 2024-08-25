#include "Window.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Shared/String.h>
#include <System/EventHandler.h>

#include "../Controls/Control.h"

namespace Editor
{
	Window::Window(UString name)
	{
		_name = name;
	}

	Window::~Window()
	{
		_dockParent = nullptr;
	}

	void Window::update()
	{
		bool opened = true;
		ImGui::Begin(ToStdString(_name).c_str(), &opened);
		updateControls();
		ImGui::End();
	}
}