#pragma once

#include <Shared/String.h>
#include <Shared/List.h>

#include "WindowManager.h"
#include "../Controls/Container.h"

namespace Editor
{
	enum class DockDirection
	{
		None,
		Up,
		Down,
		Left,
		Right
	};

	class Control;

	class Window: public Container
	{
		friend class WindowManager;

	private:
		unsigned int _id = -1;
		UString _name;
		Window* _parent = nullptr;
		Core::List<Window*> _children;
		DockDirection _dockDirection = DockDirection::None;
		float _splitSize = 0.5f;
		int _width = 400;
		int _height = 300;

		void update();

	public:
		Window(UString name);
		virtual ~Window();

		UString getName() { return _name; };

		void addWindow(Window* wnd);
		void removeWindow(Window* wnd);

		int getWidth() { return _width; }
		void setWidth(int value) { _width = value; }

		int getHeight() { return _height; }
		void setHeight(int value) { _height = value; }

		DockDirection getDockDirection() { return _dockDirection; };
		void setDockDirection(DockDirection dockDirection) { _dockDirection = dockDirection; };

		float getSplitSize() { return _splitSize; }
		void setSplitSize(float value) { _splitSize = value; }
	};
}