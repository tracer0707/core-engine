#pragma once

#include <functional>

#include <Shared/String.h>

#include "Control.h"

namespace Core
{
	class Application;
}

namespace Editor
{
	class FileInput : public Control
	{
	private:
		Core::String _filePath = "";
		Core::Application* _application = nullptr;

	public:
		FileInput(Core::Application* application);
		virtual ~FileInput();

		Core::String getFilePath() { return _filePath; }
		void setFilePath(Core::String value) { _filePath = value; }

		virtual int getControlType();
		virtual void update();
	};
}