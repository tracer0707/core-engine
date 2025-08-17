#pragma once

#include <functional>

#include <Shared/List.h>
#include <Shared/String.h>

#include "WindowList.h"

namespace Core
{
	class Time;
	class Renderer;
	class AssetManager;
	class InputManager;
	class EventHandler;
}

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
		Core::Time* _time = nullptr;
		Core::Renderer* _renderer = nullptr;
		Core::AssetManager* _assetManager = nullptr;
		Core::InputManager* _inputManager = nullptr;
		Core::EventHandler* _eventHandler = nullptr;

		Core::List<Window*> _windows;
		std::function<void()> _onDock = nullptr;

		MenuBar* _menuBar = nullptr;

		Window* addWindow(const char* name);

	public:
		WindowManager();
		~WindowManager();

		void setTime(Core::Time* value) { _time = value; }
		void setRenderer(Core::Renderer* value) { _renderer = value; }
		void setAssetManager(Core::AssetManager* value) { _assetManager = value; }
		void setInputManager(Core::InputManager* value) { _inputManager = value; }
		void setEventHandler(Core::EventHandler* value) { _eventHandler = value; }

		Core::Time* getTime() { return _time; }
		Core::Renderer* getRenderer() { return _renderer; }
		Core::AssetManager* getAssetManager() { return _assetManager; }
		Core::InputManager* getInputManager() { return _inputManager; }
		Core::EventHandler* getEventHandler() { return _eventHandler; }

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
}