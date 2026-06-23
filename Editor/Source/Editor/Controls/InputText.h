#pragma once

#include <functional>
#include <string>

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class InputText : public Control
	{
	private:
		std::string _text = "";
		std::string _prevText = "";

		float _actualWidth = 0.0f;
        float _actualHeight = 0.0f;

		std::function<void(Core::String)> _onValueChanged = nullptr;

	public:
		InputText();
		virtual ~InputText();

		virtual float getWidth() const;
        virtual float getHeight() const;

		virtual ControlType getControlType() const { return ControlType::InputText; }
		virtual void update();

		Core::String getValue() const { return _text; }
		void setValue(Core::String value) { _text = value.std_str(); }

		void setOnValueChanged(std::function<void(Core::String)> value) { _onValueChanged = value; }
	};
}