#pragma once

#include "Window.h"

namespace Editor
{
	class Button;
	class LinearLayout;

	class CSGEditWindow : public Window
	{
		private:
			LinearLayout* _layoutMain = nullptr;
			Button* _csgSelectBtn = nullptr;

			void activateAll(bool active);
			void enableAll(bool enable);

		public:
			CSGEditWindow(WindowManager* parent);
			virtual ~CSGEditWindow();

			virtual void invalidate();
	};
} // namespace Editor