#pragma once

#include <Shared/List.h>

#include "Control.h"

namespace Editor
{
	class MenuBar: public Control
	{
	private:
		bool _isMainMenu = false;
		float _height = 0;

	public:
		MenuBar();
		virtual ~MenuBar();

		virtual void update();

		void setIsMainMenu(bool value) { _isMainMenu = value; }
		bool getIsMainMenu() { return _isMainMenu; }

		float getHeight() { return _height; }
	};
}