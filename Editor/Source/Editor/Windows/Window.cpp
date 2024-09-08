#include "Window.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Shared/String.h>
#include <System/EventHandler.h>
#include <Shared/Hash.h>

#include "../Controls/Control.h"

namespace Editor
{
	Window::Window(UString name)
	{
		_name = name;
		_id = Core::Hash::getHash(ToStdString(name));
	}

	Window::~Window()
	{
		
	}

	DockArea Window::dock(DockDirection dockDirection, unsigned int relativeTo, float splitSize)
	{
		ImGuiDir dir;

		switch (dockDirection)
		{
			case DockDirection::None: dir = ImGuiDir_None; break;
			case DockDirection::Left: dir = ImGuiDir_Left; break;
			case DockDirection::Right: dir = ImGuiDir_Right; break;
			case DockDirection::Up: dir = ImGuiDir_Up; break;
			case DockDirection::Down: dir = ImGuiDir_Down; break;
			default: dir = ImGuiDir_None; break;
		}

		if (dockDirection == DockDirection::None) return _dockArea;

		if (relativeTo == 0) relativeTo = 1013603228;
		_dockArea.area1 = ImGui::DockBuilderSplitNode(relativeTo, dir, splitSize, nullptr, &_dockArea.area2);
		return _dockArea;
	}

	void Window::update()
	{
		ImGuiWindowFlags _flags = 0;
		ImGuiDockNodeFlags _dockFlags = 0;

		if (!_hasTitle)
		{
			_flags |= ImGuiWindowFlags_NoTitleBar;
			_dockFlags |= ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoDockingOverOther;
		}

		if (!_canAcceptDocking)
		{
			_dockFlags |= ImGuiDockNodeFlags_NoDockingOverMe;
		}

		if (!_canDock)
		{
			_dockFlags |= ImGuiDockNodeFlags_NoDocking;
		}

		ImGuiWindowClass _wndClass;
		_wndClass.DockNodeFlagsOverrideSet = _dockFlags;
		ImGui::SetNextWindowClass(&_wndClass);

		bool opened = true;
		ImGui::Begin(ToStdString(_name).c_str(), &opened, _flags);
		updateControls();
		ImGui::End();
	}
}