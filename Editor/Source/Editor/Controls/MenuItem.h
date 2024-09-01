#pragma once

#include <functional>

#include <Shared/List.h>
#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class MenuItem : public Control
	{
		friend class MenuBar;

	private:
		UString _text = "";
		
		std::function<void()> _onClick = nullptr;

	public:
		MenuItem(UString text) { _text = text; }
		~MenuItem();

		virtual void update();

		UString getText() { return _text; }
		void setText(UString value) { _text = value; }

		void setOnClick(std::function<void()> callback) { _onClick = callback; }
	};
}