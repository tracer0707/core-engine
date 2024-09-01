#pragma once

#include <Shared/List.h>

#include "Control.h"

namespace Editor
{
	class TreeView : public Control
	{
	public:
		TreeView();
		virtual ~TreeView();

		virtual void update();
	};
}