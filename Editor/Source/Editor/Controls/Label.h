#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Editor
{
	class Label : public Control
	{
	private:
		Core::String _text = "";

	public:
		Label();
		Label(Core::String text);
		virtual ~Label();

		Core::String getText() { return _text; }
		void setText(Core::String value) { _text = value; }

        virtual float getWidth();
        virtual float getHeight();

		virtual int getControlType();
		virtual void update();
	};
}