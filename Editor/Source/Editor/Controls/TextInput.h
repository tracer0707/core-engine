#pragma once

#include <functional>
#include <string>

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class TextInput : public Control
	{
	private:
		std::string _text = "";

	public:
		TextInput();
		virtual ~TextInput();

		virtual int getControlType();
		virtual void update();

		Core::String getText();
		void setText(Core::String value);
	};
}