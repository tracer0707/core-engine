#pragma once

#include "Window.h"

namespace Editor
{
	class Button;
	class Gizmo;

	class ToolWindow : public Window
	{
	private:
		Button* _saveBtn = nullptr;
		Button* _selectBtn = nullptr;
		Button* _translateBtn = nullptr;
		Button* _rotateBtn = nullptr;
		Button* _scaleBtn = nullptr;
		Button* _boundsBtn = nullptr;
		Button* _localSpaceBtn = nullptr;
		Button* _worldSpaceBtn = nullptr;
		Gizmo* _gizmo = nullptr;

	public:
		ToolWindow(WindowManager* parent);
		virtual ~ToolWindow();

		virtual void init();
		virtual void invalidate();

		void setGizmo(Gizmo* value) { _gizmo = value; }
	};
}