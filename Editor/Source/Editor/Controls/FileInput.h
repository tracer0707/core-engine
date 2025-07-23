#pragma once

#include <functional>
#include <string>

#include <Shared/String.h>

#include "Control.h"

namespace Editor
{
	class FileInput : public Control
	{
	private:
		std::string _id = "";
		UString _filePath = "";

	public:
		FileInput();
		virtual ~FileInput();

		virtual void update();

		UString getFilePath() { return _filePath; }
		void setFilePath(UString value) { _filePath = value; }
	};
}