#pragma once

#include <Core/Shared/String.h>

#include "Control.h"

namespace Core
{
	class Application;
}

namespace Editor
{
	class InputText;
    class LinearLayout;

	class InputFile : public Control
	{
	private:
		Core::Application* _application = nullptr;
		InputText* _textInput = nullptr;
        LinearLayout* _layout = nullptr;

	public:
		InputFile(Core::Application* application);
		virtual ~InputFile();

		virtual float getWidth() const;
        virtual void setWidth(float value);

        virtual float getHeight() const;
        virtual void setHeight(float value);

		Core::String getFilePath() const;
		void setFilePath(Core::String value);

		virtual ControlType getControlType() const { return ControlType::InputFile; }
		virtual void update();
	};
}