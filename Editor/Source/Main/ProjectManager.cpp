#include "ProjectManager.h"

#include <filesystem>

#include <Core/Shared/Path.h>
#include <Core/Renderer/Renderer.h>

#include "../Editor/Font.h"
#include "../Editor/Windows/FullscreenWindow.h"
#include "../Editor/Controls/LinearLayout.h"
#include "../Editor/Controls/FileInput.h"
#include "../Editor/Controls/Label.h"
#include "../Editor/Controls/Button.h"
#include "../Editor/Controls/ListView.h"

#include "../Serialization/RecentProjectList.h"

namespace Editor
{
    /* WINDOW */

    ProjectManager::MainWindow::MainWindow(ProjectManager* app) : Window(app, "Project Manager", 700, 500)
    {
        RecentProjectList::load();

        _mainLayout = new LinearLayout(LayoutDirection::Vertical);
        _mainLayout->setStretchX(true);
        _mainLayout->setStretchY(true);
        
        LinearLayout* _layout = new LinearLayout(LayoutDirection::Vertical);
        _layout->setVerticalAlignment(LayoutAlignment::Start);
        _layout->setHorizontalAlignment(LayoutAlignment::Center);
        _layout->getStyle().paddingX = 20;
        _layout->getStyle().paddingY = 20;

        LinearLayout* _listLayout = new LinearLayout(LayoutDirection::Vertical);
        _listLayout->getStyle().paddingX = 20;
        _listLayout->getStyle().paddingY = 20;
        _listLayout->setStretchX(true);
        _listLayout->setStretchY(true);

        LinearLayout* _buttonsLayout = new LinearLayout();
        _buttonsLayout->setHorizontalAlignment(LayoutAlignment::Center);

        Label* _label = new Label("Project location");
        FileInput* _fileInput = new FileInput(_application);
        Button* _openBtn = new Button("Open");
        Button* _quitBtn = new Button("Quit");

        ListView* listView = new ListView();

        for (auto& it : RecentProjectList::getProjectList())
        {
            auto path = std::filesystem::path(it.std_str());
            LinearLayout* ll = new LinearLayout(LayoutDirection::Vertical);
            ll->setObjectTag(0, &it);
            ll->setStretchX(true);
            ll->getStyle().paddingX = 5;
            ll->getStyle().paddingY = 5;
            Label* lbl1 = new Label(path.filename().generic_string());
            Label* lbl2 = new Label(it);
            ll->addControl(lbl1);
            ll->addControl(lbl2);
            listView->addControl(ll);
        }

        listView->setOnItemClick([=](Control* item) {
            Core::String* path = (Core::String*)item->getObjectTag(0);
            app->setSelectedProject(*path);
            app->stop(false);
        });

        _buttonsLayout->addControl(_openBtn);
        _buttonsLayout->addControl(_quitBtn);

        _layout->addControl(_label);
        _layout->addControl(_fileInput);
        _layout->addControl(_buttonsLayout);

        _listLayout->addControl(listView);

        _openBtn->setOnClick([app, _fileInput]()
        {
            Core::String selectedPath = _fileInput->getFilePath();

            app->setSelectedProject(selectedPath);

            if (!RecentProjectList::getProjectList().contains(selectedPath))
            {
                RecentProjectList::getProjectList().add(selectedPath);
                RecentProjectList::save();
            }

            app->stop(false);
        });

        _quitBtn->setOnClick([app]()
        {
            app->stop(true);
        });

        _mainLayout->addControl(_layout);
        _mainLayout->addControl(_listLayout);

        _wnd = new FullscreenWindow();
        _wnd->addControl(_mainLayout);
    }

    ProjectManager::MainWindow::~MainWindow()
    {
        delete _wnd;
        _wnd = nullptr;
    }

    void ProjectManager::MainWindow::update() {}

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

        _wnd->setOnClose([this]()
        {
            _isRunning = false;
            _forceClosed = true;
        });

        _mainFont = new Font(Core::Path::combine(std::filesystem::current_path().generic_string(), "Editor/Fonts/Roboto-Regular.ttf"), 15.0f);
        _mainFont->setDefault();
    }

    void ProjectManager::destroy()
    {
        delete _mainFont;

        _mainFont = nullptr;
        _wnd = nullptr;
    }
}