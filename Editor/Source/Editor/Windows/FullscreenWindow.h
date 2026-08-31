#pragma once

#include "Window.h"

#include "../Controls/Container.h"

namespace Editor
{
	class WindowManager;
	class FullscreenWindow : public Container
	{
	private:
	public:
		FullscreenWindow() = default;
		~FullscreenWindow() = default;

		void update(const char* id, uint32_t width, uint32_t height);
	};
}