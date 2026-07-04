#pragma once

#include "Window.h"

namespace Editor
{
	class WindowManager;
	class LinearLayout;
	class Control;

	class ObjectWindow : public Window
	{
	private:
		LinearLayout* _layoutMain = nullptr;

	public:
		ObjectWindow(WindowManager* parent);
		virtual ~ObjectWindow();
	};
}