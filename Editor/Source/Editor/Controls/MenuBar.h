#pragma once

#include <Core/Shared/List.h>

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

		virtual int getControlType() const;
		virtual void update();

		void setIsMainMenu(bool value) { _isMainMenu = value; }
		bool getIsMainMenu() const { return _isMainMenu; }

		float getHeight() const { return _height; }
	};
}