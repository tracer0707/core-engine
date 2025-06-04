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

		LinearLayout* _layoutMain = nullptr;
		Button* _csgAddModelBtn = nullptr;

		void activateAll(bool active);
		void enableAll(bool enable);
		
	public:
		CSGObjectWindow();
		virtual ~CSGObjectWindow();

		static const char* NAME;

		virtual void invalidate();
	};
}