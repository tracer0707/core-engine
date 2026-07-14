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
			Button* _csgEditVerticesBtn = nullptr;
			Button* _csgEditEdgesBtn = nullptr;
			Button* _csgEditFacesBtn = nullptr;

		public:
			CSGEditWindow(WindowManager* parent);
			virtual ~CSGEditWindow();

			virtual void invalidate();
	};
} // namespace Editor