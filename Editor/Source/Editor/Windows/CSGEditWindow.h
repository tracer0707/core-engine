#pragma once

#include "Window.h"

namespace Editor
{
	class Button;
	class LinearLayout;
	class CSGModifier;

	class CSGEditWindow : public Window
	{
	private:
		CSGModifier* _modifier = nullptr;

		LinearLayout* _layoutMain = nullptr;
		Button* _csgSelectBtn = nullptr;

		void activateAll(bool active);
		void enableAll(bool enable);

	public:
		CSGEditWindow(WindowManager* parent);
		virtual ~CSGEditWindow();

		virtual void invalidate();
	};
}