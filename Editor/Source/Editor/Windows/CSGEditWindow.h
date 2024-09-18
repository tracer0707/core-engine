#pragma once

#include "Window.h"

namespace Editor
{
	class LinearLayout;
	class CSGModifier;

	class CSGEditWindow : public Window
	{
	private:
		CSGModifier* _modifier = nullptr;

		LinearLayout* layoutMain = nullptr;

		void disableAll();

	public:
		CSGEditWindow(CSGModifier* modifier);
		virtual ~CSGEditWindow();
	};
}