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

		std::function<void()> renderFunction = nullptr;
		std::function<void()> renderUIFunction = nullptr;

		int oldtime = 0;
		int newtime = 0;
		int fps = 0;

	public:
		int createWindow(UString title, int width, int height);
		void destroyWindow();
		void* getWindow() { return window; }
		void* getRendererContext() { return rendererContext; }

		void update(bool& isRunning);
		void render();

		void setRenderFunction(std::function<void()> func) { renderFunction = func; }
		void setRenderUIFunction(std::function<void()> func) { renderUIFunction = func; }
	};
}