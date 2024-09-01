#pragma once

#include <Shared/List.h>

#include "Control.h"

namespace Editor
{
	class MenuBar: public Control
	{
	private:
		bool _isMainMenu = false;

	public:
		MenuBar();
		virtual ~MenuBar();

		virtual void update();

		void setIsMainMenu(bool value) { _isMainMenu = value; }
		bool getIsMainMenu() { return _isMainMenu; }
	};
}