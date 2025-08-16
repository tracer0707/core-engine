#pragma once

#include <System/Application.h>
#include <System/Window.h>

namespace Editor
{
	class WindowManager;

	class ProjectManager : public Core::Application
	{
	private:
		class MainWindow : public Core::Window
		{
			friend class ProjectManager;

		private:
			MainWindow();
			virtual ~MainWindow();

			virtual void update();
			virtual void render();

			WindowManager* _windowManager = nullptr;
		};

		MainWindow* _wnd = nullptr;
		
		virtual void init();
		virtual void destroy();
	};
}