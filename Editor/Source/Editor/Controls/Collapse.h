#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Collapse : public Control
	{
		private:
			Core::String _text = Core::String::Empty;

		public:
			Collapse();
			Collapse(Core::String text);
			virtual ~Collapse();

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			virtual ControlType getControlType() const { return ControlType::Collapse; }
			virtual void update();
	};
} // namespace Editor