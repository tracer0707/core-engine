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

		Core::String getText() const { return _text; }
		void setText(Core::String value) { _text = value; }

        virtual float getWidth() const;
        virtual float getHeight() const;

		virtual int getControlType() const;
		virtual void update();
	};
}