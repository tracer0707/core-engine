#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Collapse : public Control
	{
		private:
			float _actualWidth = 0.0f;
			float _actualHeight = 0.0f;

			Core::String _text = Core::String::Empty;

		public:
			Collapse();
			Collapse(Core::String text);
			virtual ~Collapse();

			virtual float getWidth() const;
			virtual float getHeight() const;

			Core::String getText() const { return _text; }
			void setText(Core::String value) { _text = value; }

			virtual ControlType getControlType() const { return ControlType::Collapse; }
			virtual void update();
	};
} // namespace Editor