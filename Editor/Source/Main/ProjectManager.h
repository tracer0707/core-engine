#pragma once

namespace Core
{
	class DeviceContext;
}

namespace Editor
{
	class ProjectManager
	{
	private:
		bool isRunning = false;

		Core::DeviceContext* ctx = nullptr;

		int init();
		void loop();
		void destroy();

	public:
		int run();
	};
}