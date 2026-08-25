#pragma once

#include <functional>

#include <Core/Shared/List.h>
#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class MenuItem : public Control
	{
		friend class MenuBar;

	private:
		Core::String _text = "";
		
		std::function<void()> _onClick = nullptr;

	public:
		MenuItem(Core::String text);
		~MenuItem();

		virtual ControlType getControlType() const { return ControlType::MenuItem; }
		virtual void measure() const;
		virtual void update();

		Core::String getText() const { return _text; }
		void setText(Core::String value) { _text = value; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
}