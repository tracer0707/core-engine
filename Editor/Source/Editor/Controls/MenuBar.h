#pragma once

#include <functional>

#include <Shared/List.h>
#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class MenuItem
	{
	private:
		UString _text;
		Core::List<MenuItem*> _menuItems;

		std::function<void()> _onClick = nullptr;

	public:
		MenuItem(UString text) { _text = text; }
		~MenuItem();

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
		void update();

		void addMenuItem(MenuItem* value) { _menuItems.add(value); }
		void addSeparator() { _menuItems.add(new MenuItem("-")); }
	};

	class MenuBar: public Control
	{
	private:
		bool _isMainMenu = false;

		Core::List<MenuItem*> _menuItems;

		virtual void update();

	public:
		MenuBar();
		virtual ~MenuBar();

		void setIsMainMenu(bool value) { _isMainMenu = value; }
		bool getIsMainMenu() { return _isMainMenu; }

		void addMenuItem(MenuItem* value) { _menuItems.add(value); }
		void addSeparator() { _menuItems.add(new MenuItem("-")); }
	};
}