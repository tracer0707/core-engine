#pragma once

#include <functional>
#include <string>

#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class TextInput : public Control
	{
	private:
		UString _text = "";

		char _buffer[256] = "";

	public:
		TextInput();
		virtual ~TextInput();

		virtual int getControlType();
		virtual void update();

		UString getText();
		void setText(UString value);
	};
}