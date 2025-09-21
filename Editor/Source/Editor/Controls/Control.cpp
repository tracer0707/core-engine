#include "Control.h"

#include <Core/Shared/Uuid.h>

namespace Editor
{
	Control::Control() : Container()
	{
		_id = Core::Uuid::create().to_string();
	}

	Control::~Control()
	{
		_parent = nullptr;
	}
}