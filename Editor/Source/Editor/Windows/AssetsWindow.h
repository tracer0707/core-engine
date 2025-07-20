#pragma once

#include "Window.h"

namespace Editor
{
	class AssetsWindow : public Window
	{
	private:
	public:
		AssetsWindow(WindowManager* parent);
		virtual ~AssetsWindow() = default;

		static const char* NAME;
	};
}