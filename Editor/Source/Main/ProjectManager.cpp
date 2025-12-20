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

        LinearLayout* _layout = new LinearLayout(LayoutDirection::Horizontal);
        _layout->setHorizontalAlignment(LayoutHorizontalAlignment::Center);

        _listLayout = new LinearLayout(LayoutDirection::Vertical);

        Button* _openBtn = new Button("Open project");
        Button* _quitBtn = new Button("Quit");

        ListView* listView = new ListView();

        for (auto& it : Serialization::RecentProjectList::getProjectList())
        {
            auto path = fs::path(it.std_str());
            LinearLayout* ll = new LinearLayout(LayoutDirection::Vertical);
            ll->setObjectTag(0, &it);
            ll->getStyle().paddingX = 5;
            ll->getStyle().paddingY = 5;
            ll->setHeight(50);
            Label* lbl1 = new Label(path.filename().generic_string());
            Label* lbl2 = new Label(it);
            ll->addControl(lbl1);
            ll->addControl(lbl2);
            listView->addControl(ll);
        }

        listView->setOnItemClick([=](Control* item) {
            Core::String* path = (Core::String*)item->getObjectTag(0);
            app->initProject(*path);
            app->setSelectedProject(*path);
            app->stop(false);
        });

        _layout->addControl(_openBtn);
        _layout->addControl(_quitBtn);

        _layout->setHeight(32.0f);

        _listLayout->addControl(listView);

        _openBtn->setOnClick([=]() {
            app->getEventHandler()->addEvent([=] {
                if (_fsDlg != nullptr) return;

                _fsDlg = new FileSystemDialog(app);
                _fsDlg->setShowFiles(false);

                _fsDlg->setOnClose([=]() { _fsDlg = nullptr; });

                _fsDlg->setOnPathSelected([=](Core::String fileName) {
                    app->initProject(fileName);
                    app->setSelectedProject(fileName);

                    if (!Serialization::RecentProjectList::getProjectList().contains(fileName))
                    {
                        Serialization::RecentProjectList::getProjectList().add(fileName);
                        Serialization::RecentProjectList::save();
                    }

                    app->stop(false);
                });
            });
        });

        _quitBtn->setOnClick([app]() { app->stop(true); });

        _mainLayout->addControl(_listLayout);
        _mainLayout->addControl(_layout);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_mainLayout);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete _wnd;
        _wnd = nullptr;
    }

    void ProjectManager::MainWindow::update()
    {
        _mainLayout->setHeight(_height);
        _listLayout->setHeight(_height - 85.0f);
    }

    void ProjectManager::MainWindow::render()
    {
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
        _mainFont = new Font(Core::Path::combine(fs::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();

        float baseFontSize = 15.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f;

        static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_16_FK, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF(
            Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts", FONT_ICON_FILE_NAME_FK).std_str().c_str(),
            iconFontSize, &icons_config, icons_ranges);
        
        Font::rebuildFonts();
    }

    void ProjectManager::destroy()
    {
        delete _mainFont;

        _mainFont = nullptr;
        _wnd = nullptr;
    }

    void ProjectManager::initProject(Core::String value)
    {
        fs::path _rootPath = fs::path(value.std_str());
        fs::path _contentPath = fs::path(Core::Path::combine(value, "Content").std_str());
        fs::path _libPath = fs::path(Core::Path::combine(value, "Library").std_str());
        fs::path _libContentPath = fs::path(Core::Path::combine(_libPath.generic_string(), "Content").std_str());
        fs::path _libCachePath = fs::path(Core::Path::combine(_libPath.generic_string(), "Cache").std_str());
        fs::path _libCacheThumbnailsPath = fs::path(Core::Path::combine(_libCachePath.generic_string(), "Thumbnails").std_str());

        Core::List<fs::path> _dirsToCreate;
        _dirsToCreate.add(_contentPath);
        _dirsToCreate.add(_libPath);
        _dirsToCreate.add(_libContentPath);
        _dirsToCreate.add(_libCachePath);
        _dirsToCreate.add(_libCacheThumbnailsPath);

        for (auto& p : _dirsToCreate)
        {
            if (!fs::exists(p))
            {
                fs::create_directories(p);
            }
        }
    }
} // namespace Editor