#pragma once

#include <Core/System/Application.h>
#include <Core/System/Window.h>
#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

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
			MainWindow(ProjectManager* app);
			virtual ~MainWindow();

			Core::List<Core::String> _recentProjects;

			FullscreenWindow* _wnd = nullptr;

			virtual void update();
			virtual void render();
		};

		Core::String _selectedProject = "";

		Font* _mainFont = nullptr;
		MainWindow* _wnd = nullptr;
		
		virtual void init();
		virtual void destroy();

	public:
		Core::String getSelectedProject() { return _selectedProject; }
		void setSelectedProject(Core::String value) { _selectedProject = value; }
	};
}