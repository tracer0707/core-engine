#pragma once

#include <functional>

#include "../Shared/String.h"

namespace Core
{
	class DeviceContext
	{
	private:
		void* window = nullptr;
		void* rendererContext = nullptr;

	public:
		int createWindow(UString title, int width, int height);
		void setWindowTitle(UString title);
		void destroyWindow();
		void* getWindow() { return window; }
		void* getRendererContext() { return rendererContext; }

		void update(bool& isRunning);
		void renderUiBegin();
		void renderUiEnd();
		void swapWindow();
	};
}