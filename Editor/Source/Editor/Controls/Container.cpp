#include "Container.h"

#include <System/EventHandler.h>

#include "Control.h"

namespace Editor
{
	void Container::addControl(Control* value)
	{
		EVENT({
			_controls.add(value);
			value->_parent = this;
		}, =);
	}

	void Container::removeControl(Control* value)
	{
		EVENT({
			_controls.remove(value);
			value->_parent = nullptr;
		}, =);
	}

	void Container::removeControl(int index)
	{
		EVENT({
			Control * control = _controls.get(index);
			control->_parent = nullptr;
			_controls.removeAt(index);
		}, =);
	}

	void Container::updateControls()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}
}