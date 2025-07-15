#pragma once

#include <functional>

#include "../Shared/String.h"

namespace Core
{
	class DeviceContext
	{
	private:
		void* window = nullptr;
		
		int windowWidth = 0;
		int windowHeight = 0;

	public:
		int createWindow(UString title, int width, int height);
		void setWindowTitle(UString title);
		void destroyWindow();
		void* getWindow() { return window; }
		
		int getWindowWidth() { return windowWidth; }
		int getWindowHeight() { return windowHeight; }

		void updateWindow(bool& isRunning);
		void swapWindow();
	};
}