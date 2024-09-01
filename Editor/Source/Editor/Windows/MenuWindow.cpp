#include "MenuWindow.h"

#include "../Controls/MenuBar.h"

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

		menuFile->addMenuItem(menuFileNewScene);
		menuFile->addSeparator();
		menuFile->addMenuItem(menuExit);

		_menuBar->addMenuItem(menuFile);

		addControl(_menuBar);
	}

	MenuWindow::~MenuWindow()
	{
	}
}