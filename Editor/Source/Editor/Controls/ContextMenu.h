#pragma once

#include <Core/Shared/String.h>
#include <Core/Shared/List.h>

#include "Control.h"

namespace Editor
{
	class MenuItem;

	class ContextMenu : public Control
	{
		private:
			Core::List<MenuItem*> _menuItems;

		public:
			ContextMenu();
			virtual ~ContextMenu();

			virtual int getControlType() const;
			virtual void update();

			void open();
	};
} // namespace Editor