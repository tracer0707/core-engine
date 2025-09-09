#pragma once

#include <System/Application.h>
#include <System/Window.h>

namespace Editor
{
	class WindowManager;
	class FullscreenWindow;
	class Font;

	class ProjectManager : public Core::Application
	{
	private:
		class MainWindow : public Core::Window
		{
			friend class ProjectManager;

		private:
			MainWindow(Core::Application* app);
			virtual ~MainWindow();

			Core::Application* _app = nullptr;
			FullscreenWindow* _wnd = nullptr;

			virtual void update();
			virtual void render();
		};

		Font* _mainFont = nullptr;
		MainWindow* _wnd = nullptr;
		
		virtual void init();
		virtual void destroy();
	};
}