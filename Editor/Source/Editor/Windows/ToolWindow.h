#pragma once

#include "Window.h"

namespace Editor
{
	class Button;

	class ToolWindow : public Window
	{
	private:
		Button* _selectBtn = nullptr;
		Button* _translateBtn = nullptr;
		Button* _rotateBtn = nullptr;
		Button* _scaleBtn = nullptr;
		Button* _boundsBtn = nullptr;
		Button* _localSpaceBtn = nullptr;
		Button* _worldSpaceBtn = nullptr;

	public:
		ToolWindow(WindowManager* parent);
		virtual ~ToolWindow();

		virtual void invalidate();
	};
}