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

		float _actualWidth = 0.0f;
        float _actualHeight = 0.0f;

		std::function<void(Core::String)> _onTextChanged = nullptr;

	public:
		TextInput();
		virtual ~TextInput();

		virtual float getWidth() const;
        virtual float getHeight() const;

		virtual ControlType getControlType() const { return ControlType::TextInput; }
		virtual void update();

		Core::String getText() const;
		void setText(Core::String value);

		void setOnTextChanged(std::function<void(Core::String)> value) { _onTextChanged = value; }
	};
}