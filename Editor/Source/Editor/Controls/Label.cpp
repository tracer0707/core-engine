#include "Label.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ControlList.h"

namespace Editor
{
	Label::Label() : Control() {}

	Label::Label(Core::String text) : Control()
	{
		_text = text;
	}

	Label::~Label()
	{
	}

	void Label::update()
	{
		if (!_visible) return;

		std::string _str = _text.std_str();
		ImGui::Text(_str.c_str());
	}

	float Label::getWidth() const
    {
		if (_width == 0.0f)
		{
            ImVec2 textSize = ImGui::CalcTextSize(_text.std_str().c_str());
            return textSize.x;
		}

        return _width;
    }

    float Label::getHeight() const
    {
        if (_height == 0.0f)
        {
            ImVec2 textSize = ImGui::CalcTextSize(_text.std_str().c_str());
            return textSize.y;
        }

        return _height;
    }

    int Label::getControlType() const
	{
		return CONTROL_LABEL;
	}
}