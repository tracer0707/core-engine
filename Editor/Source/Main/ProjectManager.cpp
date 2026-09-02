#include "ProjectManager.h"

#include <filesystem>
#include <imgui.h>

#include <Core/Shared/Path.h>
#include <Core/Renderer/Renderer.h>
#include <Core/System/EventHandler.h>

#include "FileSystemDialog.h"

#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/Label.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/ListView.h"
#include "../Editor/Controls/Separator.h"

#include "../Serialization/RecentProjectList.h"

#include "../Shared/IconsForkAwesome.h"

namespace fs = std::filesystem;

namespace Editor
{
	/* WINDOW */

	ProjectManager::MainWindow::MainWindow(ProjectManager* app) : Window(app, "Project Manager", 700, 500)
	{
		Serialization::RecentProjectList::load();

		_mainLayout = new LinearLayout(LayoutDirection::Vertical);
		_mainLayout->getStyle().paddingX = 20;
		_mainLayout->getStyle().paddingY = 20;
		_mainLayout->setWrapMode(LayoutWrapMode::NoWrap);
		_mainLayout->setFitWidth(LayoutFitMode::FitAvailable);

		LinearLayout* _layout = new LinearLayout(LayoutDirection::Horizontal);
		_layout->setHorizontalAlignment(LayoutHorizontalAlignment::Center);
		_layout->setVerticalAlignment(LayoutVerticalAlignment::Middle);
		_layout->setFitWidth(LayoutFitMode::FitAvailable);
		_layout->setFitHeight(LayoutFitMode::FitContent);
		_layout->setWrapMode(LayoutWrapMode::NoWrap);

		_listLayout = new LinearLayout(LayoutDirection::Vertical);
		_listLayout->setWrapMode(LayoutWrapMode::NoWrap);
		_listLayout->setFitWidth(LayoutFitMode::FitAvailable);

		Button* _createBtn = new Button("Create project");
		Button* _openBtn = new Button("Open project");
		Button* _quitBtn = new Button("Quit");

		_createBtn->setSize(100.0f, 24.0f);
		_openBtn->setSize(100.0f, 24.0f);
		_quitBtn->setSize(100.0f, 24.0f);

		ListView* listView = new ListView();

		for (auto& it : Serialization::RecentProjectList::getProjectList())
		{
			LinearLayout* ll = new LinearLayout(LayoutDirection::Vertical);
			ll->setObjectTag(0, &it);
			ll->getStyle().paddingX = 5;
			ll->getStyle().paddingY = 5;
			ll->setHeight(50);
			Label* lbl1 = new Label(Core::Path::toUtf8(it.filename()));
			Label* lbl2 = new Label(Core::Path::toUtf8(it));
			ll->addControl(lbl1);
			ll->addControl(lbl2);
			listView->addControl(ll);
		}

		listView->setOnItemClick([this, app](Control* item) {
			fs::path* path = (fs::path*)item->getObjectTag(0);
			app->initProject(*path);
			app->setSelectedProject(*path);
			app->stop(false);
		});

		_layout->addControl(_createBtn);
		_layout->addControl(_openBtn);
		_layout->addControl(_quitBtn);

		_listLayout->addControl(listView);

		_createBtn->setOnClick([this, app]() {
			app->getEventHandler()->addEvent([this, app] { openOrCreateProjectDlg(false); });
		});

		_openBtn->setOnClick([this, app]() {
			app->getEventHandler()->addEvent([this, app] { openOrCreateProjectDlg(true); });
		});

		_quitBtn->setOnClick([app]() { app->stop(true); });

		_mainLayout->addControl(_listLayout);
		_mainLayout->addControl(_layout);

		_wnd = new FullscreenWindow();
		_wnd->addControl(_mainLayout);
	}

	void ProjectManager::MainWindow::openOrCreateProjectDlg(bool open)
	{
		if (_fsDlg != nullptr) return;

		Core::String dlgTitle = open ? "Open Project" : "Create Project";
		FileSystemDialogType dlgType = open ? FileSystemDialogType::Open : FileSystemDialogType::Save;

		_fsDlg = new FileSystemDialog(_application, dlgTitle, dlgType);
		_fsDlg->setShowFiles(false);

		_fsDlg->setOnClose([this]() { _fsDlg = nullptr; });

		_fsDlg->setOnPathSelected([this](Core::List<fs::path> fileNames) {
			((ProjectManager*)_application)->initProject(fileNames[0]);
			((ProjectManager*)_application)->setSelectedProject(fileNames[0]);

			if (!Serialization::RecentProjectList::getProjectList().contains(fileNames[0]))
			{
				Serialization::RecentProjectList::getProjectList().add(fileNames[0]);
				Serialization::RecentProjectList::save();
			}

			((ProjectManager*)_application)->stop(false);
		});
	}

	ProjectManager::MainWindow::~MainWindow()
	{
		delete _wnd;
		_wnd = nullptr;
	}

	void ProjectManager::MainWindow::update()
	{
		_mainLayout->setHeight((float)_height);
		_listLayout->setHeight((float)_height - 75.0f);

		_renderer->setViewportSize(_width, _height);
		_renderer->clear(C_CLEAR_COLOR | C_CLEAR_DEPTH, Core::Color(0.1f, 0.1f, 0.1f, 1.0f));

		_renderer->beginUI();
		_wnd->update("Project Manager", _width, _height);
		_renderer->endUI();
	}

	/* PROJECT MANAGER */

	void ProjectManager::init()
	{
		_wnd = new MainWindow(this);

		_wnd->setOnClose([this]() {
			_isRunning = false;
			_forceClosed = true;
		});

		ImGuiIO& io = ImGui::GetIO();
		_mainFont = new Font(fs::current_path() / fs::path("Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
		_mainFont->setDefault();

		float baseFontSize = 15.0f;
		float iconFontSize = baseFontSize * 2.0f / 3.0f;

		static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_16_FK, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(
			Core::Path::toUtf8(fs::current_path() / fs::path("Editor/Fonts") / fs::path(FONT_ICON_FILE_NAME_FK)).c_str(),
			iconFontSize, &icons_config, icons_ranges);

		Font::rebuildFonts();
	}

	void ProjectManager::destroy()
	{
		delete _mainFont;

		_mainFont = nullptr;
		_wnd = nullptr;
	}

	void ProjectManager::initProject(const fs::path& value)
	{
		fs::path _contentPath = value / fs::path("Content");

		Core::List<fs::path> _dirsToCreate;
		_dirsToCreate.add(_contentPath);

		for (auto& p : _dirsToCreate)
		{
			if (!fs::exists(p))
			{
				fs::create_directories(p);
			}
		}
	}
} // namespace Editor