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
		UString _filePath = "";
		Core::Application* _application = nullptr;

	public:
		FileInput(Core::Application* application);
		virtual ~FileInput();

		UString getFilePath() { return _filePath; }
		void setFilePath(UString value) { _filePath = value; }

		virtual void update();
	};
}