#pragma once

namespace Editor
{
	class MenuBar;

	class MainMenu
	{
	private:
		MenuBar* _menuBar = nullptr;

	public:
		MainMenu();
		~MainMenu();

		MenuBar* getMenuBar() { return _menuBar; }
	};
}