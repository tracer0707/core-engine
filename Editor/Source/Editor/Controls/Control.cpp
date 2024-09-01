#include "Control.h"

namespace Editor
{
	Control::Control() : Container() {}

	Control::~Control()
	{
		_parent = nullptr;
	}
}