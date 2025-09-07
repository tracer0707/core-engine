#include "Container.h"

#include "Control.h"

namespace Editor
{
	Container::Container() {}

	Container::~Container()
	{
		clear();
	}

	void Container::clear()
	{
		for (int i = 0; i < _controls.count(); ++i)
		{
			Control* control = _controls.get(i);
			control->clear();
			control->_parent = nullptr;
			delete control;
		}

		_controls.clear();
	}

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

	void Container::updateControls()
	{
		for (auto it : _controls)
		{
			it->update();
		}
	}
}