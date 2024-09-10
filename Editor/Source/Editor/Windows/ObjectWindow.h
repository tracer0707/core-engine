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

		bool setModifier(UString name);
		void disableAll();

	public:
		ObjectWindow();
		virtual ~ObjectWindow();
	};
}