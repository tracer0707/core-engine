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
		std::string _prevText = "";

		std::function<void(Core::String)> _onTextChanged = nullptr;

	public:
		TextInput();
		virtual ~TextInput();

		virtual int getControlType();
		virtual void update();

		Core::String getText();
		void setText(Core::String value);

		void setOnTextChanged(std::function<void(Core::String)> value) { _onTextChanged = value; }
	};
}