#pragma once

#include <Shared/String.h>

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

	class DockArea
	{
	public:
		unsigned int area1 = 0;
		unsigned int area2 = 0;
	};

	class Control;

	class Window: public Container
	{
		friend class WindowManager;

	private:
		unsigned long long _id = -1;

		DockArea _dockArea;

		UString _name;
		int _width = 400;
		int _height = 300;

		bool _visible = true;
		bool _hasTitle = true;
		bool _canAcceptDocking = true;
		bool _canDock = true;

		void update();

	public:
		Window(UString name);
		virtual ~Window();

		UString getName() { return _name; };

		void setVisible(bool value) { _visible = value; }
		bool getVisible() { return _visible; }

		int getWidth() { return _width; }
		void setWidth(int value) { _width = value; }

		int getHeight() { return _height; }
		void setHeight(int value) { _height = value; }

		void setHasTitle(bool value) { _hasTitle = value; }
		bool getHasTitle() { return _hasTitle; }

		void setCanAcceptDocking(bool value) { _canAcceptDocking = value; }
		bool getCanAcceptDocking() { return _canAcceptDocking; }

		void setCanDock(bool value) { _canDock = value; }
		bool getCanDock() { return _canDock; }

		unsigned long long getId() { return _id; }

		DockArea dock(DockDirection dockDirection, unsigned int relativeTo, float splitSize);
	};
}