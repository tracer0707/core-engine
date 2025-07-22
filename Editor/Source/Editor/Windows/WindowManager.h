#pragma once

#include <functional>

#include <Shared/List.h>
#include <Shared/String.h>

#include "WindowList.h"

namespace Editor
{
	class MenuBar;
	class Window;
	class AssetsWindow;
	class CSGEditWindow;
	class CSGObjectWindow;
	class GizmoWindow;
	class HierarchyWindow;
	class InspectorWindow;
	class ObjectWindow;
	class SceneWindow;
	class ProjectManagerWindow;

	class WindowManager
	{
	private:
		MenuBar* _menuBar = nullptr;
		Core::List<Window*> _windows;
		std::function<void()> _onDock = nullptr;

		Window* addWindow(const char* name);

	public:
		WindowManager();
		~WindowManager();

		void invalidateAll();

		template <typename T>
		T addWindow() {}

		Window* getWindow(const char* name);
		void setOnDock(std::function<void()> value) { _onDock = value; }

		bool isMouseCaptured();

		MenuBar* getMenuBar() { return _menuBar; }
		void setMenuBar(MenuBar* value) { _menuBar = value; }

		void update(int width, int height);
	};

	template <>
	inline AssetsWindow* WindowManager::addWindow<AssetsWindow*>()
	{
		return (AssetsWindow*)addWindow(ASSETS_WINDOW);
	}

	template <>
	inline CSGEditWindow* WindowManager::addWindow<CSGEditWindow*>()
	{
		return (CSGEditWindow*)addWindow(CSG_EDIT_WINDOW);
	}

	template <>
	inline CSGObjectWindow* WindowManager::addWindow<CSGObjectWindow*>()
	{
		return (CSGObjectWindow*)addWindow(CSG_OBJECT_WINDOW);
	}

	template <>
	inline GizmoWindow* WindowManager::addWindow<GizmoWindow*>()
	{
		return (GizmoWindow*)addWindow(GIZMO_WINDOW);
	}

	template <>
	inline HierarchyWindow* WindowManager::addWindow<HierarchyWindow*>()
	{
		return (HierarchyWindow*)addWindow(HIERARCHY_WINDOW);
	}

	template <>
	inline InspectorWindow* WindowManager::addWindow<InspectorWindow*>()
	{
		return (InspectorWindow*)addWindow(INSPECTOR_WINDOW);
	}

	template <>
	inline ObjectWindow* WindowManager::addWindow<ObjectWindow*>()
	{
		return (ObjectWindow*)addWindow(OBJECT_WINDOW);
	}

	template <>
	inline SceneWindow* WindowManager::addWindow<SceneWindow*>()
	{
		return (SceneWindow*)addWindow(SCENE_WINDOW);
	}

	template <>
	inline ProjectManagerWindow* WindowManager::addWindow<ProjectManagerWindow*>()
	{
		return (ProjectManagerWindow*)addWindow(PROJECT_MANAGER_WINDOW);
	}
}