#pragma once

#include <functional>

#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class FileInput : public Control
	{
	private:
		UString _filePath = "";

	public:
		FileInput();
		virtual ~FileInput();

		UString getFilePath() { return _filePath; }
		void setFilePath(UString value) { _filePath = value; }

		virtual void update();
	};
}