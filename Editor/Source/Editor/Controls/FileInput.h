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

	class FileInput : public Control
	{
	private:
		Core::Application* _application = nullptr;
		TextInput* _textInput = nullptr;

	public:
		FileInput(Core::Application* application);
		virtual ~FileInput();

		Core::String getFilePath();
		void setFilePath(Core::String value);

		virtual int getControlType();
		virtual void update();
	};
}