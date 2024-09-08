#include "ToolWindow.h"

#include <Assets/Texture.h>
#include <Shared/Path.h>
#include <Shared/String.h>

#include "../Controls/MenuBar.h"
#include "../Controls/MenuItem.h"
#include "../Controls/Separator.h"
#include "../Controls/Button.h"
#include "../Controls/LinearLayout.h"

namespace Editor
{
	ToolWindow::ToolWindow() : Window("Main Menu")
	{
		/* Main menu */
		_menuBar = new MenuBar();
		_menuBar->setIsMainMenu(true);

		/* File */
		MenuItem* menuFile = new MenuItem("File");
		MenuItem* menuFileNewScene = new MenuItem("New scene");
		MenuItem* menuExit = new MenuItem("Exit");

		menuFile->addControl(menuFileNewScene);
		menuFile->addControl(new Separator());
		menuFile->addControl(menuExit);

		_menuBar->addControl(menuFile);

		addControl(_menuBar);

		/* Tools */

		LinearLayout* layout1 = new LinearLayout(LayoutDirection::Horizontal);
		LinearLayout* layout2 = new LinearLayout(LayoutDirection::Horizontal);
		LinearLayout* layoutMain = new LinearLayout(LayoutDirection::Vertical);

		layout1->setAlignmentHorizontal(LayoutAlignment::Center);
		layout2->setAlignmentHorizontal(LayoutAlignment::Center);

		layoutMain->addControl(layout1);
		layoutMain->addControl(layout2);

		/* CSG tool */

		Button* csgTool = new Button();
		Core::Texture* csgToolImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/csg.png"), Core::TextureFormat::RGBA);
		csgTool->setSize(32, 32);
		csgTool->setImage(csgToolImage);

		layout1->addControl(csgTool);

		/* CSG cube */

		Button* csgCube = new Button();
		Core::Texture* csgCubeImage = Core::Texture::loadFromFile(Core::Path::combine(Core::Path::getExePath(), "Editor/Icons/csg/cube.png"), Core::TextureFormat::RGBA);
		csgCube->setSize(32, 32);
		csgCube->setImage(csgCubeImage);

		layout2->addControl(csgCube);

		addControl(layoutMain);
	}

	ToolWindow::~ToolWindow()
	{
	}
}