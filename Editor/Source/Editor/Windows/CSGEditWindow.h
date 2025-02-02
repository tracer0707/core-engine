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
		CSGEditWindow();
		virtual ~CSGEditWindow();

		static UString NAME;

		void checkControls();
	};
}