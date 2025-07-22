#pragma once

namespace Core
{
	class DeviceContext;
}

namespace Editor
{
	class WindowManager;

	class ProjectManager
	{
	private:
		bool isRunning = false;

		Core::DeviceContext* ctx = nullptr;
		WindowManager* windowManager = nullptr;

		int init();
		void loop();
		void destroy();

	public:
		int run();
	};
}