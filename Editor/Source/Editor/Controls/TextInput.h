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
		std::string _id = "";
		UString _text = "";

		char _buffer[256] = "";

	public:
		TextInput();
		virtual ~TextInput();

		virtual void update();

		UString getText();
		void setText(UString value);
	};
}