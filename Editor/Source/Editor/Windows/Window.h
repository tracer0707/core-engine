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

		void update();

	public:
		Window(UString name);
		virtual ~Window();

		UString getName() { return _name; };

		int getWidth() { return _width; }
		void setWidth(int value) { _width = value; }

		int getHeight() { return _height; }
		void setHeight(int value) { _height = value; }

		unsigned long long getId() { return _id; }

		DockArea dock(DockDirection dockDirection, unsigned int relativeTo, float splitSize);
	};
}