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
		while (_children.count() > 0)
		{
			removeWindow(_children.get(0));
		}
	}

	void Window::addWindow(Window* wnd)
	{
		assert(wnd->_parent == nullptr && "Window has already been added to another parent window");

		EVENT({
			_children.add(wnd);
			wnd->_parent = this;
		}, =);
	}

	void Window::removeWindow(Window* wnd)
	{
		assert(wnd->_parent == this && "Child window is not owned by this window");

		EVENT({
			_children.remove(wnd);
			wnd->_parent = nullptr;
		}, =);
	}

	void Window::update()
	{
		bool opened = true;
		ImGui::Begin(ToStdString(_name).c_str(), &opened);
		updateControls();
		ImGui::End();
	}
}