#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Label : public Control
	{
	private:
		Core::String _text = Core::String::Empty;

	public:
		Label();
		Label(Core::String text);
		virtual ~Label();

		Core::String getText() const { return _text; }
		void setText(Core::String value) { _text = value; }

		virtual ControlType getControlType() const { return ControlType::Label; }
		virtual void measure() const;
		virtual void update();
	};
}