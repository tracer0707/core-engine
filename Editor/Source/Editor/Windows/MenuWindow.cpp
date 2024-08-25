#include "MenuWindow.h"

#include "../Controls/MenuBar.h"

namespace Editor
{
	MenuWindow::MenuWindow() : Window("Main Menu")
	{
		_menuBar = new MenuBar();
		_menuBar->setIsMainMenu(true);

		addControl(_menuBar);
	}

	MenuWindow::~MenuWindow()
	{
	}
}