#pragma once

#include "Window.h"

namespace Editor
{
	class LinearLayout;
	class Control;

	class ObjectWindow : public Window
	{
	private:
		LinearLayout* layoutMain = nullptr;

		bool setModifier(uint32_t name);
		void disableAll();

	public:
		ObjectWindow();
		virtual ~ObjectWindow();
	};
}