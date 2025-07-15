#pragma once

namespace Core
{
	class DeviceContext;
}

class ProjectManager
{
private:
	bool isRunning = false;

	Core::DeviceContext* ctx = nullptr;

	int init();
	void loop();
	void destroy();

public:
	int run(Core::DeviceContext* context);
};