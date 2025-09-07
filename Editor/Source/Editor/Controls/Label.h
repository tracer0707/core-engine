#pragma once

#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Label : public Control
	{
	private:
		UString _text = "";

	public:
		Label();
		Label(UString text);
		virtual ~Label();

		UString getText() { return _text; }
		void setText(UString value) { _text = value; }

		virtual int getControlType();
		virtual void update();
	};
}