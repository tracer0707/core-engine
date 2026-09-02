#pragma once

#include <Core/System/Application.h>
#include <Core/System/Window.h>
#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include <Core/Shared/Path.h>

namespace Editor
{
	class WindowManager;
	class FullscreenWindow;
	class Font;
	class LinearLayout;
	class FileSystemDialog;

	class ProjectManager : public Core::Application
	{
		private:
			class MainWindow : public Core::Window
			{
					friend class ProjectManager;

				private:
					MainWindow(ProjectManager* app);
					virtual ~MainWindow();

					Core::List<fs::path> _recentProjects;

					LinearLayout* _mainLayout = nullptr;
					LinearLayout* _listLayout = nullptr;
					FullscreenWindow* _wnd = nullptr;
					FileSystemDialog* _fsDlg = nullptr;

					void openOrCreateProjectDlg(bool open);

					virtual void update();
			};

			fs::path _selectedProject;

			Font* _mainFont = nullptr;
			MainWindow* _wnd = nullptr;

			virtual void init();
			virtual void destroy();

		public:
			fs::path getSelectedProject() const { return _selectedProject; }
			void setSelectedProject(const fs::path& value) { _selectedProject = value; }
			void initProject(const fs::path& value);
	};
} // namespace Editor