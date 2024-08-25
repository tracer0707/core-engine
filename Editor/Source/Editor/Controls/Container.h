#pragma once

#include <Shared/List.h>

namespace Editor
{
	class Control;

	class Container
	{
	protected:
		Core::List<Control*> _controls;

	public:
		Container();
		~Container();

		void addControl(Control* value);
		void removeControl(Control* value);
		Control* getControl(int index) { return _controls.get(index); }
		int getControlsCount() { return _controls.count(); }

		void updateControls();
	};
}