#include "Container.h"

#include "Control.h"

namespace Editor
{
	void Container::addControl(Control* value)
	{
		_controls.add(value);
		value->_parent = this;
	}

	void Container::removeControl(Control* value)
	{
		_controls.remove(value);
		value->_parent = nullptr;
	}

	void Container::removeControl(int index)
	{
		Control* control = _controls.get(index);
		control->_parent = nullptr;
		_controls.removeAt(index);
	}

	void Container::updateControls()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}
}