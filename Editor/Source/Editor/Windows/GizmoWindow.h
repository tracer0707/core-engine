#pragma once

#include "Window.h"

namespace Editor
{
	class Button;

	class GizmoWindow : public Window
	{
	private:
		Button* _selectBtn = nullptr;
		Button* _translateBtn = nullptr;
		Button* _rotateBtn = nullptr;
		Button* _scaleBtn = nullptr;
		Button* _localSpaceBtn = nullptr;
		Button* _worldSpaceBtn = nullptr;

	public:
		GizmoWindow(WindowManager* parent);
		virtual ~GizmoWindow();

		virtual void invalidate();

		void updateCurrentToolButtonState();
	};
}