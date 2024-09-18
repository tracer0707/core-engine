#pragma once

#include "Window.h"

namespace Editor
{
	class Button;
	class LinearLayout;
	class CSGModifier;

	class CSGObjectWindow : public Window
	{
	private:
		CSGModifier* _modifier = nullptr;

		LinearLayout* layoutMain = nullptr;
		Button* _csgAddModelBtn = nullptr;

		void disableAll();
		
	public:
		CSGObjectWindow(CSGModifier* modifier);
		virtual ~CSGObjectWindow();
	};
}