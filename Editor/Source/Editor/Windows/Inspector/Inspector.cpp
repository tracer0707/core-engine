#include "Inspector.h"

namespace Editor
{
	Inspector::~Inspector()
	{
		if (_onDestroy != nullptr)
		{
			_onDestroy();
		}
	}
} // namespace Editor