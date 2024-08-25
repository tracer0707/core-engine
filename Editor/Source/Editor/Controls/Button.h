#pragma once

#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Button: public Control
	{
	private:
		UString text = "";

		virtual void update();

	public:
		Button();
		virtual ~Button();

		UString getText() { return text; }
		void setText(UString value) { text = value; }
	};
}