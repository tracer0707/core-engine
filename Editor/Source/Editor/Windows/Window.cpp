#include "Window.h"
#include "WindowManager.h"

#include "../Controls/MenuBar.h"
#include "../Controls/Control.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Shared/String.h>
#include <System/EventHandler.h>

namespace Editor
{
	Window::Window(WindowManager* parent, const char* name)
	{
		_name = name;
		_id = Core::Uuid::create();
		_parent = parent;
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

		if (dockDirection == DockDirection::None)
		{
			_dockArea.area1 = relativeTo;
			return _dockArea;
		}
		else
		{
			if (relativeTo == 0) relativeTo = 1013603228;
			_dockArea.area1 = ImGui::DockBuilderSplitNode(relativeTo, dir, splitSize, nullptr, &_dockArea.area2);
			return _dockArea;
		}
	}

	void Window::update()
	{
		if (!_visible) return;

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

		if (!_canResize)
		{
			_flags |= ImGuiWindowFlags_NoResize;
		}

		if (!_canMove)
		{
			_flags |= ImGuiWindowFlags_NoMove;
		}

		ImGuiWindowClass _wndClass;
		_wndClass.DockNodeFlagsOverrideSet = _dockFlags;
		ImGui::SetNextWindowClass(&_wndClass);

		if (_fixedWidth != FLT_MAX || _fixedHeight != FLT_MAX)
		{
			float w = _fixedWidth != FLT_MAX ? _fixedWidth : _width;
			float h = _fixedHeight != FLT_MAX ? _fixedHeight : _height;
			_fixedWidth = FLT_MAX;
			_fixedHeight = FLT_MAX;

			ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		}

		if (_fixedPositionX != FLT_MAX || _fixedPositionY != FLT_MAX)
		{
			float x = _fixedPositionX != FLT_MAX ? _fixedPositionX : _positionX;
			float y = _fixedPositionY != FLT_MAX ? _fixedPositionY : _positionY;
			_fixedPositionX = FLT_MAX;
			_fixedPositionY = FLT_MAX;

			ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Always);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(_style.paddingX, _style.paddingY));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, _style.borderSize);
		ImGui::Begin(_name, &_visible, _flags);
		
		int cw = ImGui::GetContentRegionAvail().x;
		int ch = ImGui::GetContentRegionAvail().y;

		if (cw != _clientWidth || ch != _clientHeight)
		{
			EVENT(
			{
				onResize(cw, ch);

				if (_onResize != nullptr)
				{
					_onResize(cw, ch);
				}
			}, =);
		}

		_isHovered = ImGui::IsWindowHovered();

		_positionX = ImGui::GetWindowPos().x;
		_positionY = ImGui::GetWindowPos().y;

		MenuBar* menuBar = _parent->getMenuBar();

		if (menuBar != nullptr)
		{
			_positionY += menuBar->getHeight();
		}

		_width = ImGui::GetWindowWidth();
		_height = ImGui::GetWindowHeight();

		_clientWidth = cw;
		_clientHeight = ch;

		onUpdate();
		updateControls();

		ImGui::End();
		ImGui::PopStyleVar(2);
	}
}