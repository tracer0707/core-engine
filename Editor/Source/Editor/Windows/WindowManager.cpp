#include "WindowManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Core/Shared/String.h>
#include <Core/System/EventHandler.h>

#include "Window.h"
#include "ContentWindow.h"
#include "CSGEditWindow.h"
#include "CSGObjectWindow.h"
#include "GizmoWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "ObjectWindow.h"
#include "SceneWindow.h"

#include "../Controls/MenuBar.h"

namespace Editor
{
	const int MAIN_MENU_SIZE = 19;

	WindowManager::WindowManager(EditorApp* ed)
	{
		_app = ed;

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		ImGui::GetStyle().ItemSpacing = ImVec2(4, 4);
	}

	WindowManager::~WindowManager()
	{
		if (_menuBar != nullptr)
		{
			delete _menuBar;
			_menuBar = nullptr;
		}

		for (int i = 0; i < _windows.count(); ++i)
		{
			Window* window = _windows.get(i);
			delete window;
		}

		_windows.clear();
	}

	void WindowManager::initWindows()
	{
		for (auto win : _windows)
		{
			win->init();
		}
	}

	void WindowManager::invalidateAll()
	{
		_eventHandler->addEvent([this] {
			for (int i = 0; i < _windows.count(); ++i)
			{
				Window* window = _windows.get(i);
				window->invalidate();
			}
		});
	}

	Window* WindowManager::addWindow(std::string name)
	{
		Window* window = nullptr;

		if (name == CONTENT_WINDOW)
		{
			window = new ContentWindow(this);
		}
		else if (name == CSG_EDIT_WINDOW)
		{
			window = new CSGEditWindow(this);
		}
		else if (name == CSG_OBJECT_WINDOW)
		{
			window = new CSGObjectWindow(this);
		}
		else if (name == GIZMO_WINDOW)
		{
			window = new GizmoWindow(this);
		}
		else if (name == HIERARCHY_WINDOW)
		{
			window = new HierarchyWindow(this);
		}
		else if (name == INSPECTOR_WINDOW)
		{
			window = new InspectorWindow(this);
		}
		else if (name == OBJECT_WINDOW)
		{
			window = new ObjectWindow(this);
		}
		else if (name == SCENE_WINDOW)
		{
			window = new SceneWindow(this);
		}

		assert(window != nullptr && "Unknown window");

		_windows.add(window);

		return window;
	}

	Window* WindowManager::getWindow(std::string name)
	{
		for (int i = 0; i < _windows.count(); ++i)
		{
			Window* window = _windows.get(i);
			if (window->getName() == name)
			{
				return window;
			}
		}

		return nullptr;
	}

	bool WindowManager::isMouseCaptured()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

	void WindowManager::update(int width, int height)
	{
		bool open = true;
		ImGuiID dockID = ImGui::GetID("HUB_DockSpace");

		ImGui::SetNextWindowPos(ImVec2(0, MAIN_MENU_SIZE));
		ImGui::SetNextWindowSize(ImVec2(width, height - MAIN_MENU_SIZE));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Main", &open,
					 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
						 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
						 ImGuiWindowFlags_NoBackground);

		ImGui::PopStyleVar(3);

		if (ImGui::DockBuilderGetNode(dockID) == nullptr)
		{
			ImGui::DockBuilderRemoveNode(dockID);
			ImGui::DockBuilderAddNode(dockID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockID, ImVec2(width, height));

			if (_onDock != nullptr) _onDock();

			for (auto it : _windows)
			{
				ImGui::DockBuilderDockWindow(it->_name.c_str(), it->_dockArea.area1);
			}

			ImGui::DockBuilderFinish(dockID);
		}

		ImGui::DockSpace(dockID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);
		ImGui::End();

		if (_menuBar != nullptr) _menuBar->update();

		for (auto it : _windows)
		{
			it->update();
		}
	}
} // namespace Editor