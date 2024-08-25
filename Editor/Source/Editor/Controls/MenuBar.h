#pragma once

#include "Control.h"

namespace Editor
{
	class MenuBar: public Control
	{
	private:
		bool _isMainMenu = false;

		virtual void update();

	public:
		MenuBar();
		virtual ~MenuBar();

		void setIsMainMenu(bool value) { _isMainMenu = value; }
		bool getIsMainMenu() { return _isMainMenu; }
	};
}