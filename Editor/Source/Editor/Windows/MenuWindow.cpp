#include "MenuWindow.h"

#include "../Controls/MenuBar.h"
#include "../Controls/MenuItem.h"
#include "../Controls/Separator.h"

namespace Editor
{
	MenuWindow::MenuWindow() : Window("Main Menu")
	{
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
	}

	MenuWindow::~MenuWindow()
	{
	}
}