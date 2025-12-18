#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Core
{
	class Application;
}

namespace Editor
{
	class TextInput;
    class LinearLayout;

	class FileInput : public Control
	{
	private:
		Core::Application* _application = nullptr;
		TextInput* _textInput = nullptr;
        LinearLayout* _layout = nullptr;

	public:
		FileInput(Core::Application* application);
		virtual ~FileInput();

		virtual float getWidth() const;
        virtual void setWidth(float value);

        virtual float getHeight() const;
        virtual void setHeight(float value);

		Core::String getFilePath() const;
		void setFilePath(Core::String value);

		virtual int getControlType() const;
		virtual void update();
	};
}